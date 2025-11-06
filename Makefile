SRCS = main.cpp eg.cpp chromosome.cpp problem.cpp weak.cpp utils.cpp emso.cpp

main: $(SRCS)
	g++ -std=c++20 $(SRCS) -o main2

# clean:
# 	rm -f main
