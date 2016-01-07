// You can edit this code!
// Click here and start typing.
package main

import "fmt"
import "runtime"


func initvector(start,end int, v []int, c chan int){

for i := start; i < end; i++ {
	v[i] = i

} 

c <- 1
}


func main(/*argc int, argv []string*/) {

	var numCPUs= runtime.GOMAXPROCS(0)
	//var numCPUs= runtime.NumCPU()
	fmt.Println("Hello, 世界", numCPUs, " CPUs")
	
	v := make([]int,numCPUs)

	c:= make (chan int)
	for i := 0; i < numCPUs; i++ {
        	go initvector(i,i+1,v,c)

	}

	for i := 0; i < numCPUs; i++ {
		<-c
	fmt.Println("Signal ",i, " received")
	}

	for i := 0; i < numCPUs; i++ {
	fmt.Println("v[",i,"]=",v[i])
	}

}
