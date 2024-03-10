echo '----compiling'
flex tokenizer.flex
g++ -Wall -o tokenizer lex.yy.c hashtable.cpp invertedFile.cpp -lfl 
echo 'Made by Blayten Jones'
echo '----running'
/bin/rm -rf $2
/bin/rm -rf mapFile.txt
/bin/rm -rf postFile.txt
/bin/rm -rf dictFile.txt
./tokenizer $1 $2
