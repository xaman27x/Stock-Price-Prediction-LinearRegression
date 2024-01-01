# Welcome to the Stock Price Prediction Project!
# In this project, we aim to predict the estimated closing prices of a stock listed in NSE (National Stock Exchange) using historical data and advanced Linear Regression Models. Additionally, we will explore the influence of lunar phases on the stock market.

#Disclaimer! This Project is still under development phase and will require future updates.

# Let's dive into the code and uncover valuable insights into stock price movements. Feel free to explore and analyze the implemented models. If you have any questions or suggestions, don't hesitate to reach out.

# Happy coding and may your predictions be as accurate as the phases of the moon!

import yfinance as yf
import subprocess
import time
print("*-------------------------------------------------------*\n|Copyright (c) Local File=/home/aman/Desktop/NSE_codes  |\n|Author: xaman27x                                       |\n|A.R.R.                                                 |\n*-------------------------------------------------------*");

print("                                         _____ _             _      __  __ _    _                          _           _     ")
print("                                        / ____| |           | |    |  \/  | |  | |       /\               | |         (_)    ")
print("                                       | (___ | |_ ___   ___| | __ | \  / | | _| |_     /  \   _ __   __ _| |_   _ ___ _  ___ ")
print("                                        \___ \| __/ _ \ / __| |/ / | |\/| | |/ / __|   / /\ \ | '_ \ / _` | | | | / __| |/ __|")
print("                                        ____) | || (_) | (__|   <  | |  | |   <| |_   / ____ \| | | | (_| | | |_| \__ \ |\__ \\")
print("                                       |_____/ \__\___/ \___|_|\_\ |_|  |_|_|\_\\__|  /_/    \_\_| |_|\__,_|_|\__, |___/ |/___/")
print("                                                                                                             |___/           ")
print("                                                                G i t H u b :   x a m a n 2 7 x           \n");



print("                                                      A STOCK MARKET ANALYSIS MODEL USING LINEAR REGRESSION  v1.0    \n\n");

print("Please Enter the STOCK CODE of the company for which you Wish to Estimate the Closing Price. The stock code is typically a Short Abbreviation used on the Stock Exchange. For Example, if you want to predict the Closing Price for Infosys, Enter 'INFY' and SO ON\n\nSOME EXAMPLE CODES: \n  CODE     -   NAME\nTATAMOTORS - Tata Motors\nNESTLEIND - Nestle India\nADANIENT - Adani Enterprises Ltd.\nHINDUNILVR - Hindustan Unilever Ltd.")

x = input("Enter STOCK CODE: ")
y = x + ".NS"

stock = yf.Ticker(y)
stock_info = stock.info
stock_history = stock.history(period='30d')

high = float(stock_info.get('dayHigh', 'N/A'))
low = stock_info.get('dayLow', 'N/A')
market_open = stock_info.get('regularMarketOpen', 'N/A')
current_price = stock_info.get('currentPrice', 'N/A')

temp = stock_history.to_string(header=False, index=False, index_names=False).split('\n')
temp = [','.join(i.split()) for i in temp]
z = input("\nDo You Want The Recent Opening and Closing Parameters of This Stock? If Yes, type the No. of Days , Numerical Only: ")
stock_history_1 = stock.history(period=z+'d')
print("\nPAST", z, "DAYS ANALYSIS FOR CLOSING AND OPENING PRICES OF THE STOCK ALONGSIDE VITAL PARAMETERS\n")
print(stock_history_1,'\n')
print("NOTE: Past 30 Days' Stock History has been saved in a .txt file in your local machine, which will be removed after the completion of program") 

with open("data.txt", "w") as FILE:
    FILE.write("DAILY_HIGH: ")
    FILE.write(str(high))
    FILE.write("\n")
    for line in temp:
        FILE.write(line + '\n')
        
cmd1 = "cc analysis.c -o ANALYSIS"        
cmd2 = "./ANALYSIS data.txt"
subprocess.call(cmd1, shell=True)
subprocess.call(cmd2, shell=True)
