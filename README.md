## DMA Simulator (cycle-accurate simulator)
### Block-diagram

![image](https://github.com/user-attachments/assets/ef98bb37-2060-4deb-b425-e5982e8794cc)



### Input  
1. DMA_PARAMS_DESCRIPTS (num of channels, addr_width, sg_or_not (scatter-gather mode), BTT (Bytes to Transfer), burst_or_not (Support Burst Mode or Not)
2. TRANSFER_DESCRIPTS (transaction_size(Data_width), transfer_count(size of transfer (bytes)), burst_size(Burst_size for transfer), src_deice(select DEVICE MEMORY), dst_device(select DEVICE MEMORY) 
3. DEVICE MEMORY ( 0 ~ 3 )


### Output
1.  Clcok simulation
2.  Total Clcok Cycles

### Output Log (Burst)
![image](https://github.com/user-attachments/assets/9716a486-c95c-474f-bc14-59b17352e41a)


### Output Log (Non-Burst)
![image](https://github.com/user-attachments/assets/eae29f50-3a47-4656-83d5-83734b4e681b)


## Configuartion
### Header File
1. Module (Top Module)
2. DMA_SYSTEM ( has DMA_v1 and DMA_CHANNEL )
3. DMA_v1 ( has DMA_CHANNEL FOR num of CH)
4. DMA_CHANNEL

### FLOW-CHART for One-Transaction
![image](https://github.com/user-attachments/assets/d42740c3-44f9-441f-8af3-623498ac2abe)

