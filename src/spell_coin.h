#ifndef _spell_coin_H
#define _spell_coin_H

typedef struct spell_err *ErrList;

typedef enum {
  SpellComment = '@',
  SpellOK = '*',        /* Pas de fautes */
  SpellRoot = '+',      /* Indique la racine */
  SpellCompound = '-',  /* Le mot est un concatenation de 2 autres*/
  SpellMiss = '&',      /* Mot non trouve avec des solutions */
  SpellGuess = '?',     /* Mot non trouve avec des solutions  
			   [prefix+] root [-prefix] [-suffix] [+suffix] */
  SpellNone = '#'       /* Mot non trouve sans solution */
} SpellErrType;

struct spell_err {
  char* original;   /* Le fameux mot plain de fautes */
  unsigned offset;  /* Sa position dans la phrase */
  unsigned proposals_size; /* Nombre de propositions */
  char** proposals; /* Contiendra les propositions de correction 
		       Mais pas encore implemente
		     */
  ErrList next;
};

ErrList spellString(const char* str, const char* spellCmd, const char* spellDict);

#endif
