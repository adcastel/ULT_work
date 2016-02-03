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


	//fmt.Println("Vectorscal  start:",start,"end:",end)
	var posfin=pos+gran	
	for i := pos; i < posfin; i++ {
	    v[i] = v[i] * value
	}


c <- 1
}


func main(/*argc int, argv []string*/) {

	var numCPUs= runtime.GOMAXPROCS(0)
	
	var ntasks int = 10
	var nreps int = 50
	var granularity int = 1
	ntasks, _ = strconv.Atoi(os.Args[1])
	//nreps, _ = strconv.Atoi(os.Args[2])
	granularity, _ = strconv.Atoi(os.Args[2])
	var total int = ntasks*granularity
	v := make([]float32,total)
	times := make([]float64,nreps)        
	times_join := make([]float64,nreps)        
	times_crea := make([]float64,nreps)        

	var value float32 = 0.9
	
	c:= make (chan int)
	
    for t := 0; t < nreps; t++ {
        
	initvector(0,total,v)
	
	t_start := time.Now();

	for i := 0; i < total; i+=granularity {
        	go vectorscal(i,granularity, v, value,c)

	}
	t_start2 := time.Now();

	for i := 0; i < ntasks; i++ {
		<-c
	//fmt.Println("Signal ",i, " received")
	}

	t_end := time.Now();
	tim := t_end.Sub(t_start)
	tim_join := t_end.Sub(t_start2)
	tim_crea := t_start2.Sub(t_start)
	//fmt.Println(d,d.Seconds())
	times[t]=(tim.Seconds())
	times_join[t]=(tim_join.Seconds())
	times_crea[t]=(tim_crea.Seconds())
	


    }

	var min, max, avg, aux, sigma, dev float64
	var avgj float64 = float64(times_join[0]) 
	var avgcrea float64 = float64(times_crea[0]) 
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
		avgcrea += times_crea[t]
	}
	avg = aux / float64(nreps)

	for t := 0; t < nreps; t++ {
		sigma = sigma + ((times[t]-avg)*(times[t] -avg))
	}

	dev = math.Sqrt(sigma)
	
	fmt.Println(numCPUs,ntasks,avg,"[",min,"-",max,"]", dev,"Crea(",avgcrea/float64(nreps),") Join(",avgj/float64(nreps),")")

	for i := 0; i < ntasks; i++ {
		if v[i] != (float32(i)*value) {
			fmt.Println("v[",i,"]=",v[i])
		} 
		/*else {
			fmt.Println("OK")

		}*/
	}

}
