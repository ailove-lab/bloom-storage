package main

import (
	"flag"
	"fmt"
	"math/rand"
	"runtime"
	"strconv"
	"time"
	"sync"
)



func main() {



	runtime.GOMAXPROCS(runtime.NumCPU())

	flag.Parse()
	args := flag.Args()

	cnt, err := strconv.Atoi(args[0])
	if err != nil || cnt <= 0 {
		fmt.Println("usage: generator count", err)
		return
	}
	var wg sync.WaitGroup
	
	out := make(chan string, 10000)
	for j :=0; j<24; j++ {
		wg.Add(1)
		go func(){	
			var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))
			defer wg.Done()
			for i := 0; i < cnt/24; i++ {
				out<-fmt.Sprintf("%08X%08X%08X%08X",rnd.Uint32(),rnd.Uint32(),rnd.Uint32(),rnd.Uint32())
			}
		}()
	}
	
	done := make(chan bool)
	go func(){
		for {
			str, more := <-out
			if more {
				fmt.Println(str)
			} else {
				done <-true
				return
			}
		}
	}()
	wg.Wait()

	close(out)
	<-done
}
