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





func vectorscal(outer_pos, inner_start, inner_end, niterations int, v [] float32, value float32, c chan int){


	//fmt.Println(os.Getpid(),"Vectorscal  start:",pos,"end:",pos+gran)
	for i := inner_start; i < inner_end; i++ {
	    v[outer_pos*niterations+i] = v[outer_pos*niterations+i] * value
	}


	c <- 1
}

func taskcreator(it_start, it_end, numCPUs, niterations int, v [] float32, value float32, c chan int){

        channel:= make (chan int)

	var nCPUs = numCPUs
	//fmt.Println(os.Getpid(), "taskcreator  :",nCPUs, "tareas para rango externo",it_start,"-",it_end)

        var block, rest, start, end int
        block = niterations/numCPUs
        rest = niterations % numCPUs 
        
        for i := it_start; i < it_end; i++ {
		start=0
		end=0
		for ct := 0; ct < numCPUs; ct++ {
                	start = end
                	var inc int = 0
                	if ct < rest {
                        	inc = 1
                	}
               	 	end = end + block + inc

        		go vectorscal(i, start, end, niterations, v, value, channel)
		}

        	for i := 0; i < nCPUs; i++ {
                	<-channel
        		//fmt.Println("Signal ",i, " received")
        	}
	}

	c <- 1
}


func main(/*argc int, argv []string*/) {

	var numCPUs= runtime.GOMAXPROCS(0)
	
	var niterations int = 10
	var nreps int = 50
	//var granularity int = 1
	niterations, _ = strconv.Atoi(os.Args[1])
	//granularity, _ = strconv.Atoi(os.Args[2])
	//nreps, _ = strconv.Atoi(os.Args[3])
	//var total int = ntasks*granularity
	var total int = niterations*niterations
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
        block = niterations/numCPUs
        rest = niterations % numCPUs
        start = 0
        end = 0

        for i := 0; i < numCPUs; i++ {
                start = end
                var inc int = 0
                if i < rest {
                        inc = 1
                }
                end = end + block + inc
                go taskcreator(start, end, numCPUs, niterations , v, value, c)

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
	
	fmt.Println(numCPUs,niterations,avg,"[",min,"-",max,"]", dev,"Join(",avgj/float64(nreps),")")

	for i := 0; i < total; i++ {
		if v[i] != (float32(i)*value) {
			fmt.Println("v[",i,"]=",v[i])
		} 
	}

}
