/*
    wmCoinCoin, the stupidest WindowMaker dock applet
    This file is Copyright (C) 2001  fpy & pouaite & poolpy

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


 */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "global.h"
#include "coin_util.h"
#include "myprintf.h"
#include "spell_coin.h"

/* c'est pas bien de foutre des variables globales au fichier ici mais... */
int is_ispell_running = 0;
int ispell_stdin, ispell_stdout;
time_t ispell_time_last_used = 0;

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
char* convert4Spell(const unsigned char* str)
{
  unsigned char *buff, *s;
  int lastspace = 1;

  buff = malloc(strlen(str)+2); strcpy(buff, str); 
  s = buff;
  while (*s) {
    if( lastspace 
	&&( strncmp(s, "https://", 8)==0 || 
	    strncmp(s, "http://", 7)==0 || 
	    strncmp(s, "ftp://", 6)==0 )) {
      for(; s[1] && s[1]>=' '; ++s)
	*s = ' '; 
      lastspace = 0;
    } else if (*s <= ' ') {
      lastspace = 1; 
      *s = ' ';
    } else lastspace = 0;
    s++;
  }
  strcat(buff, "\n");
  //  BLAHBLAH(1, myprintf("conver4spell: s='%<GRN %s>'\n", res));
  return buff;
}

/*
  bien sur, spell_cmd ne doit pas être "rm -rf *" mais plutot un truc du genre 'ispell -ddico -a'
*/
int
launch_ispell(const char *spell_cmd)
{
  int tube_stdin[2];
  int tube_stdout[2];

  assert(is_ispell_running == 0);
  /* tout ceci a été pompée de mnière éhontée dans 'prog syst en c sous linux'..*/
  if (pipe(tube_stdin)) {
    return -1;
  }
  if (pipe(tube_stdout)) { /* rhhhoooo :-( */
    close(tube_stdin[0]); close(tube_stdin[1]);
    return -1;
  }
  switch (fork()) {
  case -1: /* arrrrg */
    {
      fprintf(stderr, "échec du fork..(%s)\n", strerror(errno));
      close(tube_stdin[0]); close(tube_stdin[1]);
      close(tube_stdout[0]); close(tube_stdout[1]);	
      return -1;
    } break;
  case 0: /* fiston (ispell) */
    {
      close(tube_stdin[1]); /* bouchage des tuyau */
      close(tube_stdout[0]);
      dup2(tube_stdin[0], STDIN_FILENO);
      dup2(tube_stdout[1], STDOUT_FILENO);

      /* a remplacer par exec !! (c'est pas beau, on voit 2 wmcoincoin + 1 ispell dans le top !!) */
      system(spell_cmd);
      exit(0);
    } break;
  default: /* pôpa (wmcc) */
    {
      close(tube_stdin[0]); /* bouchage des autres extermités des tuyaux */
      close(tube_stdout[1]);
      ispell_stdin = tube_stdin[1];
      ispell_stdout = tube_stdout[0];
      ispell_time_last_used = time(NULL);
      is_ispell_running = 1;
    } break;
  }
  return 0;
}

int
is_ispell_ready(const char* spell_cmd)
{
  if (is_ispell_running) {
    return 1;
  } else {
    return (launch_ispell(spell_cmd) == 0 ? 1 : 0);
  }
}

void
kill_ispell()
{
  is_ispell_running = 0;
  printf("kill ispell\n");
  close(ispell_stdin);
  close(ispell_stdout); /* et paf ! */
}

/* appelée régulièrement par X_loop */
int
check_if_should_kill_ispell() 
{
  if (is_ispell_running) {
    if (difftime(time(NULL), ispell_time_last_used) > 10.0) { /* au bout de 10 secondes, on libère ispell.. */
      kill_ispell();
    }
  }
  return 0;
}



ErrList spellString(const char* str, const char* spellCmd, const char* spellDict)
{
  static unsigned char *old_spellStr = NULL;
  static ErrList old_ret = NULL;
  
  unsigned char* spellStr = convert4Spell(str);


  if( old_spellStr && strcmp(spellStr, old_spellStr)==0 ) {
    /* Si la nouvelle chaine est comme l'ancienne on renvoit 
       l'ancien resultat.
    */
    free(spellStr);
    return old_ret;
  } else {
    unsigned char* spellSh;
    ErrList *end_of_ret = &old_ret;
    
    /* reinitailise le passe de cette fonction */
    if( old_spellStr )
      free(old_spellStr);
    old_spellStr = spellStr;
    freeErrList(old_ret);
    old_ret = NULL;

    /* lancement de ispell */
    spellSh = (char*)malloc(strlen(spellStr)+strlen(spellDict)+7);
    sprintf(spellSh, "%s -d%s -a", spellCmd, spellDict);

    BLAHBLAH(1 ,myprintf("running %<RED %s>\n", spellSh));

    //    if (is_ispell_running) kill_ispell();

    if (!is_ispell_ready(spellSh)) {
      free(spellSh); return NULL;
    } else {
      char buff[1024];
      unsigned char *s;
      ErrList tmp;

      free(spellSh);

      s = spellStr;
      /* envoie la chaine à ispell */
      printf("on envoie '%s' à ispell\n", s);
      while (*s) {
	if (write(ispell_stdin, s, 1) != 1) {
	  printf("erreur, il restait '%s' à envoyer à ispell...\n", s);
	  break; /* ajouter plus tard la gestion des erreurs retryables */
	}
	s++;
      }
      printf("apres ecriture: %s\n", strerror(errno));
      do {
	int i, got;
	char c;

	/* lecture de la ligne comme un gros boeuf */
	i = 0;
	while (i < 1023) {
	  do {
	    got = read(ispell_stdout, &c, 1);
	  } while (got <= 0 && errno == EINTR);
	  
	  if (c != '\n') {
	    buff[i] = c;
	  } else {
	    buff[i] = 0;
	    break;
	  }
	  i++;
	}
	buff[i] = 0;

	BLAHBLAH(1, myprintf("reponse: '%<MAG %s>' (err=%s)\n", buff, strerror(errno)));
	switch( buff[0] ) {
	case 0:
	  break;
	case SpellComment:
	case SpellOK:
	case SpellRoot:
	case SpellCompound:
	  /* on passe a la ligne suivante */
	  break;
	case SpellMiss:
	case SpellGuess:
	case SpellNone:
	  /* Super!!!! ispell a trouve une faute ... 
	     il est bon ce ispell !(?) */
	  tmp = (ErrList)malloc(sizeof(struct spell_err));
	  tmp->next = NULL;

	  /* on saute les espaces */
	  s = buff+1;
	  while (*s == ' ' && *s) s++;
	  
	  /* recherche du mot original */
	  i = 0;
	  while (s[i] > ' ') i++;
	  tmp->original = malloc(i+1);
	  strncpy(tmp->original, s, i); tmp->original[i] = 0;

	  s += i;
	  if( buff[0]!=SpellNone ) {
	    sscanf(s, "%d %d", &tmp->proposals_size,&tmp->offset);
	  } else {
	    sscanf(s, "%d", &tmp->offset);
	  }
	  /* pour l'instant je recupere pas les propositions */
	  tmp->proposals_size = 0;
	  tmp->proposals = NULL;
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
	  fprintf(stderr, "spellString: unknow option \'%c\'\n", buff[0]);
	  kill_ispell(); /* je veux pas d'un ispell tout patraque */
	  assert(0);     /* voila une triste fin */
	}
      } while (buff[0]);
    }
    ispell_time_last_used = time(NULL); /* hop ! */
    /* renvoie la liste des fautes 
     */
    return old_ret;
  }
}

