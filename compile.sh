mkdir -p build
mkdir -p bin

LIBS="-std=c99 -lm -g"
COMPILER="gcc"

echo "-> =========== COMPILING "

echo "-> CLEAN "
rm build/*
rm bin/*

echo "-> COMPILING "
# opzione -O3 ottimizza la compilazione per la velocità
# opzione -c compila senza linkare
#$COMPILER -c -o "build/vettore.o" vettore.c  $LIBS
#$COMPILER -c -o "build/contenitore.o" contenitore.c  $LIBS
#$COMPILER -c -o "build/disco.o" disco.c  $LIBS

#test driven development
#$COMPILER $LIBS ./build/*.o tdd.c -o bin/test.out

echo "-> RUN TESTS "
#./bin/test.out

echo "-> RUN MAIN PROGRAM "
#$COMPILER ./build/*.o main.c -o bin/main.out $LIBS
#nemiver bin/main.out
$COMPILER vettore.c contenitore.c disco.c main.c $LIBS -o bin/main.out
./bin/main.out

#echo "-> Visualizer "
#cp ~/NetBeansProjects/MDVisualize/dist/MDVisualize.jar .
java -jar MDVisualize.jar /home/sanjeya/Repo/physics/src/log.txt
