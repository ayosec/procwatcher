
env = Environment(CFLAGS="-Wall -D_GNU_SOURCE=1 -g")
env.ParseConfig('pkg-config --cflags --libs libproc-ng')

env.Program('procwatcher', Glob('src/*.c'))
