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
/*
  rcsid=$Id: spell_coin.c,v 1.5 2001/12/02 19:04:42 pouaite Exp $
  ChangeLog:
  $Log: spell_coin.c,v $
  Revision 1.5  2001/12/02 19:04:42  pouaite
  suppression de messages de debug...

  Revision 1.4  2001/12/02 18:11:45  pouaite
  amélioration du support de ispell (moins de pb de ralentissement du palmipede) au prix d'un vilain hack

*/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "global.h"
#include "coin_util.h"
#include "myprintf.h"
#include "spell_coin.h"

/* c'est pas bien de foutre des variables globales au fichier ici mais... */
int ispell_pid = -1; /* remplace is_ispell_running */
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
  Lance la commande 'spell_cmd -d spell_dict -a' avec des pipes 
  d'entree/sortie associes
*/
int
launch_ispell(const char *spell_cmd, const char* spell_dict)
{
  int tube_stdin[2];
  int tube_stdout[2];
  int spell_pid;

  assert(ispell_pid == -1);
  /* tout ceci a été pompée de mnière éhontée dans 'prog syst en c sous linux'..*/
  if (pipe(tube_stdin)) {
    return -1;
  }
  if (pipe(tube_stdout)) { /* rhhhoooo :-( */
    close(tube_stdin[0]); close(tube_stdin[1]);
    return -1;
  }
  /* Attention je suis co**llu je suis un male ... je prends des risques
     je fait un vfork (Vous les petits jeunes regardez pas ca c'est MAL(tm)
  */
  switch ( spell_pid = vfork() ) {
  case -1: /* arrrrg */
    {
      fprintf(stderr, "échec du vfork..(%s)\n", strerror(errno));
      close(tube_stdin[0]); close(tube_stdin[1]);
      close(tube_stdout[0]); close(tube_stdout[1]);	
      return -1;
    } break;
  case 0: /* fiston (ispell) */
    {
      int retExec;

      close(tube_stdin[1]); /* bouchage des tuyau */
      close(tube_stdout[0]);
      dup2(tube_stdin[0], STDIN_FILENO);
      dup2(tube_stdout[1], STDOUT_FILENO);

      /* Je suis toujours co**llu et je fait un execlp ... enfin la c'est
	 petit joueur (le execle serait plus rapide ... on lui donne un
	 env allege)mais j'ai peur de faire une couffe
       */
      retExec = execlp(spell_cmd, spell_cmd, "-d", spell_dict, "-a", NULL, 
		       NULL);
      if( retExec==-1 ) {
	fprintf(stderr, "échec de l'exec..(%s)\n", strerror(errno));
      }
      close(tube_stdin[0]);
      close(tube_stdout[1]);
      exit(retExec);
    } break;
  default: /* pôpa (wmcc) */
    {
      close(tube_stdin[0]); /* bouchage des autres extermités des tuyaux */
      close(tube_stdout[1]);
      ispell_stdin = tube_stdin[1];
      ispell_stdout = tube_stdout[0];
      ispell_time_last_used = time(NULL);
      ispell_pid = spell_pid;
    } break;
  }
  return 0;
}

int
is_ispell_ready(const char* spell_cmd, const char* spell_dict)
{
  if (ispell_pid!=-1) {
    return 1;
  } else {
    return (launch_ispell(spell_cmd, spell_dict) == 0 ? 1 : 0);
  }
}

void
kill_ispell()
{
  int spell_pid = ispell_pid, ret;
  ispell_pid = -1;
  BLAHBLAH(1,printf("kill ispell(%d)\n", spell_pid));
  close(ispell_stdin);
  close(ispell_stdout); /* et paf ! */
  waitpid(spell_pid, &ret, 0); /* On attends la reponse ca elimine les zombies :) */
}

/* appelée régulièrement par X_loop */
int
check_if_should_kill_ispell() 
{
  if (ispell_pid!=-1) {
    if (difftime(time(NULL), ispell_time_last_used) > 10.0) { /* au bout de 10 secondes, on libère ispell.. */
      kill_ispell();
    }
  }
  return 0;
}

static ErrList current_errlist = NULL;
static unsigned char *current_spell_string = NULL;

ErrList spellString(const char* str)
{
  static unsigned char *requested_spell_string = NULL;
  ErrList errlst;

  if (flag_spell_request) return NULL;

  errlst = NULL;
  requested_spell_string = convert4Spell(str);
  if( current_spell_string && strcmp(requested_spell_string, current_spell_string)==0) {
    /* Si la nouvelle chaine est comme l'ancienne on renvoit 
       l'ancien resultat.
    */
    free(requested_spell_string);
    errlst = current_errlist;
  } else {
    /* si les chaines commencent pareil, on renvoie les vieilles erreurs pour eviter des phenomène de clignotement .. */
    errlst = NULL;
    if (current_spell_string) {
      int l;
      l = strlen(current_spell_string);
      if (l && strncmp(requested_spell_string, current_spell_string, l)==0) {
	errlst = current_errlist;
      }
    }
    
    if( current_spell_string )
      free(current_spell_string);
    current_spell_string = requested_spell_string; 
    flag_spell_request = 1;
  }
  return errlst;
}

void
ispell_run_background(const char* spellCmd, const char* spellDict)
{
  ErrList *end_of_ret = &current_errlist;
  
  int save_errno; /* comme cette fonction est susceptible d'etre appelee depuis http.c,
		     il ne faut pas qu'elle modifie errno !
		  */
  
  save_errno = errno;
  if (flag_spell_request == 0) return;
  if (flag_spell_finished) return; /* car sinon on serait susceptible d'appeller la fonction du dessus dans un environnement multitache (et donc on accederait a errlist etc..)
					  --> au cas ou un jour coincoin deviendrait threadé (et il l'est déjà +ou- sous cygwin)) autant faire des  choses pas trop trop nazes */
  

  /* reinitailise le passe de cette fonction */
  
  freeErrList(current_errlist);
  current_errlist = NULL;

  /* lancement de ispell */

  BLAHBLAH(2 ,myprintf("running %<RED %s -d  %s -a>\n", 
		       spellCmd, spellDict));

  if (!is_ispell_ready(spellCmd, spellDict) ) {
    errno = save_errno;
    return;
  } else {
    char buff[1024];
    unsigned char *s;
    ErrList tmp;

    /*      free(spellSh);*/

    s = current_spell_string;
    /* envoie la chaine à ispell */
    BLAHBLAH(2,printf("on envoie '%s' à ispell\n", s));
    while (*s) {
      if (write(ispell_stdin, s, 1) != 1) {
	printf("erreur, il restait '%s' à envoyer à ispell...\n", s);
	break; /* ajouter plus tard la gestion des erreurs retryables */
      }
      s++;
      ALLOW_X_LOOP;
    }
    //printf("apres ecriture: %s\n", strerror(errno));
    do {
      int i, got;
      char c;

      /* lecture de la ligne comme un gros boeuf */
      i = 0;
      while (i < 1023) {
	do {
	  got = read(ispell_stdout, &c, 1);
	  ALLOW_X_LOOP;
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

      BLAHBLAH(2, myprintf("reponse ISPELL: '%<MAG %s>' (err=%s)\n", buff, strerror(errno)));
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
  flag_spell_finished = 4;  /* y'a un petit delai entre la fin de la correction et 
			      son affichage, c'est juste pour éviter un clignotement trop
			      chiant quand on tape
			      -> on decremente progressivement flag_spell_finished dans X_loop
			     */
  flag_spell_request = 0;
  errno = save_errno;
}


