#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "global.h"
#include "coin_util.h"
#include "myprintf.h"
#include "spell_coin.h"


/*
  Libere une ErrList de la memoire (c francais ca?)
 */
void freeErrList(ErrList l)
{
  ErrList tmp;
  unsigned i;
  while( l!=NULL ) {
    tmp = l;
    l = tmp->next;
    free(tmp->original);
    if( tmp->proposals!=NULL ) {
      for(i=0 ; i<tmp->proposals_size; ++i)
	free(tmp->proposals[i]);
      free(tmp->proposals);
    }
    free(tmp);
  }
}


/*
  prepare une chaine de caractere a etre traite par ispell
 */
char* convert4Spell(const char* str)
{
  char *buff, *cpt, *res;
  int lastspace = 1;

  buff = (char*)malloc(2*strlen(str)+1);
  for( cpt=buff; (*cpt=*str); ++cpt, ++str) {
    /* 1) Protection des carcteres shell */
    if( strchr(";/\\?:@&=+$#%-_.\'`\"()|!*<>~", *str) ) {
      *(cpt++) = '\\';
      *cpt = *str;
      lastspace = 1;
      /* 2) on efface les URLs */
    } else if( lastspace 
	       &&( strncmp(str, "https://", 8)==0 || 
		   strncmp(str, "http://", 7)==0 || 
		   strncmp(str, "ftp://", 6)==0 )) {
      for(; str[1] && !isspace(str[1]); ++str, ++cpt)
	*cpt = ' '; 
      *cpt = ' ';
      lastspace = 0;
    } else 
      lastspace = isspace(*str);
  }
  res = strdup(buff);
  free(buff);
  BLAHBLAH(1, myprintf("conver4spell: s='%<GRN %s>'\n", res));
  return res;
}

ErrList spellString(const char* str, const char* spellCmd, const char* spellDict)
{
  static char *old_spellStr = NULL;
  static ErrList old_ret = NULL;
  
  char* spellStr = convert4Spell(str);

  if( old_spellStr && strcmp(spellStr, old_spellStr)==0 ) {
    /* Si la nouvelle chaine est comme l'ancienne on renvoit 
       l'ancien resultat.
    */
    free(spellStr);
    return old_ret;
  } else {
    unsigned spellShLen = strlen(spellStr)+strlen(spellDict)+strlen(spellCmd)+22;
    char* spellSh;
    ErrList *end_of_ret = &old_ret;
    FILE* spellRun;

    /* reinitailise le passe de cette fonction */
    if( old_spellStr )
      free(old_spellStr);
    old_spellStr = spellStr;
    freeErrList(old_ret);
    old_ret = NULL;

    /* lancement de ispell */
    spellSh = (char*)malloc(spellShLen);
    sprintf(spellSh, "echo %s|%s -d%s -a", spellStr, spellCmd, spellDict);

    BLAHBLAH(1 ,myprintf("running %<RED echo \"%s\"|%s -d%s -a>\n",spellStr, spellCmd, spellDict));
    spellRun = popen(spellSh, "r");
    if( spellRun==NULL ) {
      /* echec au lancement */
      fprintf(stderr, "Unable to launch \"%s\"\n", spellSh);
      free(spellSh);
    } else {
      char buff[1024];
      ErrList tmp;
      int readch, i, chtmp,c;

      free(spellSh);
      while( !feof(spellRun) ) {
	/* lecture du premier caractere */
	readch = fgetc(spellRun);
	if (readch >= ' ') {
	  BLAHBLAH(1, myprintf("reponse: '%<MAG %c>'\n", readch));
	} else {
	  BLAHBLAH(1, myprintf("reponse: '%<MAG 0x%04x>' (%x)\n", readch, '\r'));
	}
	switch( readch ) {
	case -1:
	  /* c'est une erreur mais ca arrive souvent avec les pipes
	     ... les pipes SUX !!!!! */
	case '\n':
	  break;
	case SpellComment:
	case SpellOK:
	case SpellRoot:
	case SpellCompound:
	  /* on passe a la ligne suivante */
	  while( !feof(spellRun) && (c = fgetc(spellRun)) != '\n' )
	    BLAHBLAH(1, myprintf("%c", c));
	  BLAHBLAH(1, myprintf("\n"));
	  break;
	case SpellMiss:
	case SpellGuess:
	case SpellNone:
	  /* Super!!!! ispell a trouve une faute ... 
	     il est bon ce ispell !(?) */
	  tmp = (ErrList)malloc(sizeof(struct spell_err));
	  tmp->next = NULL;
	  while( (chtmp=fgetc(spellRun))==' ' );
	  for( i=0; (buff[i]=chtmp)!=' '; ++i)
	    chtmp = fgetc(spellRun);
	  buff[i] = '\0';
	  tmp->original = strdup(buff);
	  if( readch!=SpellNone )
	    fscanf(spellRun, "%d", &(tmp->proposals_size));
	  fscanf(spellRun, "%d", &(tmp->offset));
	  /* pour l'instant je recupere pas les propositions */
	  tmp->proposals_size = 0;
	  tmp->proposals = NULL;
	  while( !feof(spellRun) && (c = fgetc(spellRun)) != '\n' )
	    BLAHBLAH(1, myprintf("%c", c));
	  BLAHBLAH(1, myprintf("\n"));
	  *end_of_ret = tmp;
	  end_of_ret = &(tmp->next);
	  break;
	default:
	  /* Qu'est ce que je fout ici moi ?
	     NOTE peut etre que je devrqit en tenir compte au cas
	     ou le pgm (ki n'est pas ispell) a des retours en +?
	     (keskil renvoit aspell au fait? ... regarder son man
	      sur une machine qui a ce truc)
	  */
	  fprintf(stderr, "spellString: unknow option \'%c\'\n", readch);
	  pclose(spellRun);
	  exit(-1);
	}
      }
    }
    pclose(spellRun);
    /* renvoie la liste des fautes 
     */
    return old_ret;
  }
}

