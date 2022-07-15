import numpy as np

import matplotlib.pyplot as plt

f = open("data_4chc.hex", "r")
a = np.fromfile(f, dtype=np.uint32)

wave_len = 8000                         ## Number of samples per channels

filler_size = 1                         ## IP CONFIGURED FOR 4 CH
                                        ## Filler is 1

state = 0
idx=0
ch0 = []
ch1 = []
ch2 = []
ch3 = []
for x in a:
    if (state==0):                      ## CHECK HEADER
        if (x==0xFFFFFFFF):
            state = 1
            start_counter = 0
            hits = 0
            user =0
            ts = 0
            icnt = 0
            filler_cnt =filler_size
            wavec = wave_len
        else:
            print ("DECODE ERROR")
    elif (state==1):                    ## CHECK HEADER
        ts = x
        state = 2
    elif (state==2):
        ts += x << 32 
        state = 3        
    elif (state==3):
        start_counter = x
        state = 4
    elif (state==4):
        hits = x 
        state = 5               
    elif (state==5):
        hits += x << 32 
        state = 6
    elif (state==6):
        user = x 
        if (filler_cnt>0):
            state = 7       
        else:
            state = 8     
    elif (state==7):
        filler_cnt = filler_cnt - 1
        if (filler_cnt==0):
            state = 8
    elif (state==8):
        if icnt == 0:
            ch0.append(x&0xFFFF)
            ch1.append((x>>16)&0xFFFF)
            icnt = 1
        elif icnt == 1:
            ch2.append(x&0xFFFF)
            ch3.append((x>>16)&0xFFFF)
            icnt = 0
            if wavec == 0:
               state = 0
               idx=idx +1
            else:
                wavec = wavec-1



    
fig, axes = plt.subplots(nrows=2, ncols=1)

print("Total waveform in the file: " + str(idx))

axes[0].plot(ch0)
axes[1].plot(ch1)

plt.show()