#!/bin/sh
echo '/* ce fichier est genere autmatiquement à partir du fichier "options"' > src/options_list.h
echo '   par le script "genoptlst.sh" NE PAS EDITER !!! */' >> src/options_list.h
echo -e '#ifndef OPTIONS_LIST_H\n' >> src/options_list.h
echo -e '#define OPTIONS_LIST_H\n\n' >> src/options_list.h
echo 'typedef enum {' >> src/options_list.h

#options basiques
sed -e 's/#[^a-z].*//' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/\./_/g' | sed -e 's/^#\?/OPT_/' | sed -e 's/$/,/' | uniq >> src/options_list.h

#options dependant du site
grep '#\.[a-z]' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/\./_/g' | sed -e 's/#_/OPTS_/' | sed -e 's/$/,/' | uniq >> src/options_list.h

#options dependant du site ou changement des valeurs par defaut
grep '#\[\.\][a-z]' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/\./_/g' | sed -e 's/#\[_\]/OPTSG_/' | sed -e 's/$/,/' | uniq >> src/options_list.h

echo 'NB_WMCC_OPTIONS' >> src/options_list.h
echo -e '} wmcc_options_id;\n' >> src/options_list.h
echo -e '\n#if defined(PREFS_C)\n' >> src/options_list.h

#generation des chaines de caracteres de noms d'options
echo 'char *wmcc_options_strings[NB_WMCC_OPTIONS+1] = {' >> src/options_list.h

#options basiques
sed -e 's/#[^a-z].*//' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/^#\?/\"/' | sed -e 's/$/\",/' | uniq >> src/options_list.h

#options dependantes du site
grep '#\.[a-z]' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/#/\"/' | sed -e 's/$/\",/' | uniq >> src/options_list.h

#options dependantes du site, mais pouvant etre définies de manière globale
grep '#\[\.\][a-z]' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/#\[\.\]/!/' | uniq | awk '{ print "\""$0"\","}' >> src/options_list.h


echo -e 'NULL};\n#else\nextern char **wmcc_options_strings;\n#endif\n#endif' >> src/options_list.h
