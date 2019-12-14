HADOOP_PATH = ${HADOOP}
compile:
	${HADOOP_PATH}/bin/hadoop com.sun.tools.javac.Main MapReduceScores.java
	jar cf wc.jar MapReduceScores*.class

run: compile
	${HADOOP_PATH}/bin/hadoop fs -ls