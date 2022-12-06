import pandas as pd

"""
things to install
pip install pandas
pip install odfpy
"""

"""
HID-page is what the corisponding id goes with
So there's different pages for different types of messages
"""

#indexs
hid_usage_page = 2
hid_usage_id = 3
ps2_set_2 = 4
ps2_set_2_ext = 5


data = pd.read_excel('./usb_to_ps2.ods', engine='odf')
data["hid_usage_id"]        = data["hid_usage_id"].astype("string").apply(int, base=16)
data["ps2_set_2"]           = data["ps2_set_2"].astype("string").apply(int, base=16)
data["ps2_set_2_extension"] = data["ps2_set_2_extension"].fillna(0).astype("string").apply(int, base=16)



# generate ps2USBMapping
# PS2 keys to USB keys
outputData = [0]
data = data.sort_values("ps2_set_2") #dataframe
for row in data.itertuples():
    if(row[ps2_set_2] != 0 and 
       row[hid_usage_page] == 7):
        offBy = row[ps2_set_2] - len(outputData)
        for n in range(offBy):
            outputData.append(0);    
        outputData.append(row[hid_usage_id])
print(outputData)


# generate UsbToPS2Mapping
outputData = [0]
outputExt_Data = [0] #extension data
data = data.sort_values("hid_usage_id") #dataframe
for row in data.itertuples():
    if(row[hid_usage_id] != 0 and 
       row[hid_usage_page] == 7):
        
        offBy = row[hid_usage_id] - len(outputData)
        for n in range(offBy):
            outputData.append(0)
        
        offBy = row[hid_usage_id] - len(outputExt_Data)
        for n in range(offBy):
            outputExt_Data.append(0)
        
        if(row[ps2_set_2] != 0):
            outputData.append(row[ps2_set_2])
            outputExt_Data.append(0)
        else:
            outputData.append(row[ps2_set_2_ext])
            outputExt_Data.append(1)
print("\n\n")
print(outputData)
print("\n\n")
print(outputExt_Data)