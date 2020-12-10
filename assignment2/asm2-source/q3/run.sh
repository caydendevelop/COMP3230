# Some sample waisted parts
num_space=100

num_cars=3
num_workers=5
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=7
num_workers=19
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=1
num_workers=23
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=3
num_workers=9
./tesla_factory.out $num_cars $num_space $num_workers


# Some sample deadlock testing cases 
num_cars=1
num_space=1
num_workers=2
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=1
num_space=1
num_workers=3
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=2
num_space=1
num_workers=2
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=1
num_space=1
num_workers=5
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=1
num_space=2
num_workers=5
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=2
num_space=1
num_workers=5
./tesla_factory.out $num_cars $num_space $num_workers

num_cars=2
num_space=3
num_workers=7
./tesla_factory.out $num_cars $num_space $num_workers

