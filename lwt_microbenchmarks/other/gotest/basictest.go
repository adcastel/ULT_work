// You can edit this code!
// Click here and start typing.
package main

import "fmt"
import "runtime"
import "os"
import "strconv"


func initvector(start,end int, v [] int, value float64, c chan int){

for i := start; i < end; i++ {
	v[i] = i

} 

c <- 1
}


func main(/*argc int, argv []string*/) {

	var numCPUs= runtime.GOMAXPROCS(0)
	//var numCPUs= runtime.NumCPU()
	fmt.Println("Hello, 世界", numCPUs, " CPUs")
	var n int = 10
	n, _ = strconv.Atoi(os.Args[1])
	v := make([]int,n)
	fmt.Println("Hello, 世界",numCPUs,"CPUs and size",n)
        value:=0.9;
	c:= make (chan int)
	for i := 0; i < numCPUs; i++ {
        	go initvector(i,i+1,v,value,c)

	}

	for i := 0; i < numCPUs; i++ {
		<-c
	fmt.Println("Signal ",i, " received")
	}

	for i := 0; i < numCPUs; i++ {
	fmt.Println("v[",i,"]=",v[i])
	}

}
