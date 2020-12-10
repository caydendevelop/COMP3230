#This is a sample bash script for q2. 
#You can modify it for your benchmark experiments in q3

num_cars=1
num_space=40
num_workers=8
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=4
num_workers=8
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=16
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=32
./tesla_factory.out $num_cars $num_space $num_workers
