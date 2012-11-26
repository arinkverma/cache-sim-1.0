#!/bin/bash
SSDIR=/home/arink/workspace/simplescalar
CACHES=caches.txt
OUTPUT=output.csv

SIZE=2048
NSETS=( 32 64 128 256 512 1024 )
BLOCKS=( 8 16 32 64 )
ASSOC=( 1 2 4 8 )
POL=( l r f )

rm -rf code/*.txt

len=0
for b in "${!BLOCKS[@]}";
do
	for a in "${!ASSOC[@]}";
	do
		i=${BLOCKS[$b]}
		j=${ASSOC[$a]}
		n=$(( SIZE/(i * j) ))
		for p in "${!POL[@]}";
		do
			CONFIG[$len]="${n}:${i}:${j}:${POL[$p]}"
			len=$((len+1))
		done
	done
done



# for n in "${!NSETS[@]}";
# do
# 	for b in "${!BLOCKS[@]}";
# 	do
# 		for a in "${!ASSOC[@]}";
# 		do
# 			for p in "${!POL[@]}";
# 			do
# 			CONFIG[$len]="${NSETS[$n]}:${BLOCKS[$b]}:${ASSOC[$a]}:${POL[$p]}"
# 			len=$((len+1))
# 			done
# 		done
# 	done
# done



#compling all c codes
for i in `ls -a code/*.c`
do
	echo $i
	filename="${i%.*}"
	$SSDIR/bin/sslittle-na-sstrix-gcc -o $filename.out $i
done


#running all c codes
for i in `ls -a code/*.out`
do
	filename="${i%.*}"
	for n in $(seq 0 $((len-1)))  
	do 
		echo $n.of.$len
	   $SSDIR/simplesim-3.0/sim-cache -redir:sim "$filename:${CONFIG[$n]}:out.txt" -cache:dl1 dl1:"${CONFIG[$n]}" -cache:il1 il1:"${CONFIG[$n]}" $i code/textinput.in -f  #optional agruments
	done
done


#echo '{\n"caches": [\n' > $OUTPUT

echo 'code,il1.accesses,il1.hits,il1.misses,il1.replacements,il1.writebacks,il1.invalidations,il1.miss_rate,il1.repl_rate,il1.wb_rate,il1.inv_rate,dl1.accesses,dl1.hits,dl1.misses,dl1.replacements,dl1.writebacks,dl1.invalidations,dl1.miss_rate,dl1.repl_rate,dl1.wb_rate,dl1.inv_rate,nsets,bsize,assoc,repl' > $OUTPUT

len=0
#parsing codes and writing json
for i in `ls -a code/*.txt`
do
	len=$((len+1))
	echo $len.$i
	programPath=$(echo $i | cut -d':' -f1)
	programName="${programPath##*/}"
	#echo '"program" : "'$programName'",' >> $OUTPUT
	output=$programName','
	while read LINE
	do		
		PROP=$(echo $LINE | cut -d' ' -f1)
		name="${PROP%.*}"
		var="${PROP##* }"
		if [ "$name" = "il1" ]; then			
			val=$(echo $LINE | cut -d' ' -f2)
			#echo $val',' >> $OUTPUT
			output=$output$val','
		fi
		if [ "$name" = "dl1" ]; then			
			val=$(echo $LINE | cut -d' ' -f2)
			#echo $val',' >> $OUTPUT
			output=$output$val','
		fi

	done < $i

	nsets=$(echo $i | cut -d':' -f2)
	bsize=$(echo $i | cut -d':' -f3)
	assoc=$(echo $i | cut -d':' -f4)
	repl=$(echo $i | cut -d':' -f5)
	#echo $nsets','$bsize','$assoc','$repl'|'>> $OUTPUT
	output=$output$nsets','$bsize','$assoc','$repl
	echo $output >> $OUTPUT
done

echo '---end---' >> $OUTPUT