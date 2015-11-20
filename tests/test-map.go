package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

var storage map[*string]*string

func main() {
	
	storage = make(map[*string]*string)

	inFile, err := os.Open("test-data.txt")
	if err != nil {
		fmt.Println("Read file error: ", err)
	}
	defer inFile.Close()
	scanner := bufio.NewScanner(inFile)
	scanner.Split(bufio.ScanLines) 
	
	t1 := time.Now()
	for scanner.Scan() {
		var kv []string = strings.Split(scanner.Text(), "\t")
		// fmt.Println(kv[0],kv[1])
		storage[&kv[0]] = &kv[1]
	}
	fmt.Println(time.Since(t1))

}

// Read whole file
// buf := bytes.NewBuffer(nil)
// for _, filename := range filenames {
//   f, _ := os.Open(filename) // Error handling elided for brevity.
//   io.Copy(buf, f)           // Error handling elided for brevity.
//   f.Close()
// }
// s := string(buf.Bytes())