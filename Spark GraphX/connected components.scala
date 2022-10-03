import org.apache.spark.sql.SparkSession
// To make some of the examples work we will also need RDD
import org.apache.spark.rdd.RDD
import org.apache.spark.graphx.GraphLoader
import org.apache.spark.sql.functions._
object ReadTextFile {

    def main() : Unit = {
        val spark:SparkSession = SparkSession.builder()
            .master("local[1]")
            .appName("Spark SQL")
            .getOrCreate()   
        spark.sparkContext.setLogLevel("ERROR")
        val graph = GraphLoader.edgeListFile(sc, "file:///home/ccw/hadoop-2.10.1/input/edge_list.txt")
        val cc = graph.connectedComponents()// structure is (x,y) x is the id and y is the smallest id in that cluster
        println("Count is: ")
        println(cc.vertices.map(x => x._2).distinct.count())//count the 
        println("total count is " + cc.vertices.count())
        cc.vertices.take(100).foreach(println)
        cc.vertices.map(x => (x._2, x._2)).countByKey().toList.sortWith((x, y) => x._2 > y._2).foreach(println)//print list of cluster with counter
        println("\n\n2Count is: \n\n\n")
        val strong=graph.stronglyConnectedComponents(5)
        println(strong.vertices.map(x => x._2).distinct.count())
        
    }
}