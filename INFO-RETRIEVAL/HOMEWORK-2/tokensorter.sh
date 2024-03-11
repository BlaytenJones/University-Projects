echo '----compiling'
flex tokenizer.flex
g++ -Wall -o tokenizer lex.yy.c -lfl 
echo 'Made by Blayten Jones'
echo '----running'
/bin/rm -rf $2
mkdir $2
counter=0
breakpoint="${3:-100000}"
for file in "$1"/*
do
        tmpname=$(basename $file)
	base=$(echo "$tmpname" | sed "s/html/txt/g")
	./tokenizer $file $2/$base
	cat $2/$base >> "$2/alpha.txt"
	((counter++))
	if [ "$counter" -eq "$breakpoint" ]; then
		break
	fi
done
echo '----sorting'
grep -v '^ *$' "$2/alpha.txt" | sort -b -o "$2/alpha.txt"
uniq -c "$2/alpha.txt" > "$2/freq.txt"
cp "$2/freq.txt" "$2/alpha.txt"
sort -k1,1nr -k2,2 "$2/freq.txt" -o "$2/freq.txt"
