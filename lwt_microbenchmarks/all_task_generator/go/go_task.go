// You can edit this code!
// Click here and start typing.
package main

import "fmt"
import "runtime"
import "os"
import "strconv"
import "time"
import "math"

func initvector(start,end int, v [] float32){

for i := start; i < end; i++ {
	v[i] = float32(i)

} 

}





func vectorscal(pos, gran int, v [] float32, value float32, c chan int){


	//fmt.Println(os.Getpid(),"Vectorscal  start:",pos,"end:",pos+gran)
	var posfin=pos+gran	
	for i := pos; i < posfin; i++ {
	    v[i] = v[i] * value
	}


	c <- 1
}

func taskcreator(start, end, gran int, v [] float32, value float32, c chan int){

        channel:= make (chan int)

	var ntasks int = end-start
	//fmt.Println(os.Getpid(), "taskcreator  :",ntasks,"tareas de gran",gran)

        for i := start; i < end; i++ {
        	go vectorscal(i*gran, gran, v, value, channel)
	}

        for i := 0; i < ntasks; i++ {
                <-channel
        //fmt.Println("Signal ",i, " received")
        }


	c <- 1
}


func main(/*argc int, argv []string*/) {

	var numCPUs= runtime.GOMAXPROCS(0)
	
	var ntasks int = 10
	var nreps int = 50
	var granularity int = 1
	ntasks, _ = strconv.Atoi(os.Args[1])
	granularity, _ = strconv.Atoi(os.Args[2])
	//nreps, _ = strconv.Atoi(os.Args[3])
	var total int = ntasks*granularity
	v := make([]float32,total)
	times := make([]float64,nreps)        
	times_join := make([]float64,nreps)        

	var value float32 = 0.9
	
	c:= make (chan int)
	//fmt.Println(os.Getpid(),"empezando...")
    for t := 0; t < nreps; t++ {
        
	initvector(0,total,v)
	
	t_start := time.Now();

 	var block, rest, start, end int
        block = ntasks/numCPUs
        rest = ntasks % numCPUs
        start = 0
        end = 0

        for i := 0; i < numCPUs; i++ {
                start = end
                var inc int = 0
                if i < rest {
                        inc = 1
                }
                end = end + block + inc
                go taskcreator(start, end, granularity , v, value, c)

        }

	t_start2 := time.Now();

	for i := 0; i < numCPUs; i++ {
		<-c
	}

	t_end := time.Now();
	tim := t_end.Sub(t_start)
	tim_join := t_end.Sub(t_start2)
	times[t]=(tim.Seconds())
	times_join[t]=(tim_join.Seconds())
	


    }

	var min, max, avg, aux, sigma, dev float64
	var avgj float64 = float64(times_join[0]) 
	min = times[0] 
	max = times[0] 
	aux = times[0] 
    
	for t := 0; t < nreps; t++ {
		if times[t] < min {
			min = times[t]
		}
		if times[t] > max {
			max = times[t]
		}
		aux += times[t]
		avgj += times_join[t]
	}
	avg = aux / float64(nreps)

	for t := 0; t < nreps; t++ {
		sigma = sigma + ((times[t]-avg)*(times[t] -avg))
	}

	dev = math.Sqrt(sigma)
	
	fmt.Println(numCPUs,ntasks,avg,"[",min,"-",max,"]", dev,"Join(",avgj/float64(nreps),")")

	for i := 0; i < ntasks; i++ {
		if v[i] != (float32(i)*value) {
			fmt.Println("v[",i,"]=",v[i])
		} 
	}

}
