all:tree_builder
tree_builder:tree_builder.tab.c lex.yy.c parse_tree.h
	g++ -o tree_builder tree_builder.tab.c -ll
tree_builder.tab.c:tree_builder.y parse_tree.h
	bison tree_builder.y
	cat tree_builder.tab.c | sed 's/  __attribute__ ((__unused__))/\/\/ /g' >temp.cc
	mv temp.cc tree_builder.tab.c
lex.yy.c:tree_builder.l
	flex tree_builder.l
