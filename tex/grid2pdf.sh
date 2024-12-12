gcc toDots.c -O2 -o toDots

GRID=$1
FILE=${GRID##*/}
BASE=${FILE%.*}

./toDots $GRID > $BASE.tex
pdflatex $BASE.tex
#mv $BASE.pdf ../pdf/
rm $BASE.tex $BASE.aux $BASE.log

