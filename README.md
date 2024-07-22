## DMA Simulator (on-going)
  C++ Modeling 
  
### Input  
1. DMA_PARAMS_DESCRIPTS (num of channels, addr_width, sg_or_not (scatter-gather mode), BTT (Bytes to Transfer), burst_or_not (Support Burst Mode or Not)
2. TRANSFER_DESCRIPTS (transaction_size(Data_width), transfer_count(size of transfer (bytes)), burst_size(Burst_size for transfer), src_deice(select DEVICE MEMORY), dst_device(select DEVICE MEMORY) 
3. DEVICE MEMORY ( 0 ~ 3 )

### Output 
1. Total Clcok Cycles
2. Num of Writes , Num of Reads

## Configuartion
### Header File
1. Module (Top Module)
2. DMA_SYSTEM ( has DMA_v1 and DMA_CHANNEL )
3. DMA_v1 ( has DMA_CHANNEL FOR num of CH)
4. DMA_CHANNEL

### flow_chart for one-transaction
![image](https://github.com/user-attachments/assets/d42740c3-44f9-441f-8af3-623498ac2abe)

