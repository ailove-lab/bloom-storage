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

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))


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
	wg.Add(24)
	for j :=0; j<24; j++ {
		fmt.Println(j)
		go func(){	
			for i := 0; i < cnt/24; i++ {
				fmt.Printf("%d: %08X%08X%08X%08X\n",i, rnd.Uint32(),rnd.Uint32(),rnd.Uint32(),rnd.Uint32())
			}
			wg.Done()
		}()
	}
	wg.Wait()
	time.Sleep(1*time.Second)

}
