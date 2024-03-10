echo '----compiling'
flex query.flex
g++ -Wall -o query lex.yy.c hashtable.cpp invertedFile.cpp -lfl
echo 'Made by Blayten Jones'
echo '----Taking in query'
./query "$@"
