package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
)

func main() {
	file, err := os.Open("../../data/data-sample-1G")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		key_val := strings.Split(scanner.Text(), "\t")
		if len(key_val) == 2 {
			fmt.Print("LPUSH ", key_val[0], " \"", key_val[1], "\"\r\n")
		}
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
