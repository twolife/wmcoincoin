#!/bin/sh
echo '/* ce fichier est genere autmatiquement à partir du fichier "options"' > src/options_list.h
echo '   par le script "genoptlst.sh" NE PAS EDITER !!! */' >> src/options_list.h
echo -e '#ifndef __OPTIONS_LIST_H\n' >> src/options_list.h
echo -e '#define __OPTIONS_LIST_H\n\n' >> src/options_list.h
echo 'typedef enum {' >> src/options_list.h
sed -e 's/###.*//' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/\./_/g' | sed -e 's/#/OPT_/' | sed -e 's/$/,/' | uniq >> src/options_list.h
echo 'NB_WMCC_OPTIONS' >> src/options_list.h
echo -e '} wmcc_options_id;\n' >> src/options_list.h
echo -e '\n#ifdef __PREFS_C\n' >> src/options_list.h
echo 'static char *wmcc_options_strings[NB_WMCC_OPTIONS+1] = {' >> src/options_list.h
sed -e 's/###.*//' options | sed -e 's/#%.*//' | grep ':' | sed -e 's/:.*//' | sed -e 's/#/\"/' | sed -e 's/$/\",/' | uniq >> src/options_list.h
echo -e 'NULL};\n#endif\n#endif' >> src/options_list.h
