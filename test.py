import time
import sys

for i in range(0,10):
    sys.stdout.flush() #需要刷新缓存区,实现ue5实时获取缓存数据
    print(i)
    os.system(f'echo {i}')
    time.sleep(1)
