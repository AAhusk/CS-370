COMPILER=javac
JRE=java
FILES=Coordinator.java Producer.java Consumer.java Buffer.java
EXE=Coordinator
all:
	$(COMPILER) $(FILES)
run:
	$(JRE) $(EXE)
clean:
	rm *.class

tar:
	tar -zcvf Aaron-Huskerson-HW5.tar *.java *.txt Makefile