import org.apache.spark.sql.SparkSession
import org.apache.spark.rdd.RDD
import org.apache.spark.graphx.GraphLoader
import org.apache.spark.sql.functions._
import org.apache.spark.graphx.lib.LabelPropagation
object ReadTextFile {

    def main() : Unit = {
        val spark:SparkSession = SparkSession.builder()
            .master("local[1]")
            .appName("Spark SQL")
            .getOrCreate()   

        val graph = GraphLoader.edgeListFile(sc, "file:///home/ccw/hadoop-2.10.1/input/edge_list.txt")
        val LPA = LabelPropagation.run(graph,50)
        println("LPA count is: " + LPA.vertices.map(x => x._2).distinct.count())
        val tmp=LPA.vertices.map(x => (x._2, x._2)).countByKey().filter(_._2 > 1).toList.sortWith((x, y) => x._2 > y._2).take(21).foreach(println)
    }
}