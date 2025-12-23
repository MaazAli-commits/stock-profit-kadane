import yfinance as yf

if __name__ == "__main__":
    
    data = yf.download("AAPL", start="2012-01-01", end="2025-01-01")
    data[["Close"]].to_csv("../data/stock_data_new_2.csv")

