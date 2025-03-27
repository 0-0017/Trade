# Crypto Trading Bot

## Warning
- **Tested on Windows only!** Compatibility with other operating systems is not guaranteed.
- **Keys & Secrets Modified** To Utilize This Program, Please Add Valid Keys and Secrets to Transaction & Trade cpps.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Future Enhancements](#future-enhancements)

## Overview
This project is a **crypto trading bot** designed to interact with the **Kraken API** for automated trading. It implements **technical analysis** and trading strategies to execute buy and sell orders based on market conditions. The bot includes features for authentication, order management, and technical indicators such as **EMA, RSI, and Stochastic Oscillator**.

## Features
- **API Integration**: Uses Kraken API for retrieving market data and executing trades.
- **Authentication**: Secure API key and signature generation.
- **Trading Execution**: Open, close, and cancel orders based on technical analysis.
- **Technical Analysis**:
  - OHLC data fetching
  - RSI (Relative Strength Index)
  - EMA (Exponential Moving Average)
  - Stochastic Oscillator
- **Transaction Logging**: Saves trade details and updates external databases.

## Technologies Used
- **C++**: Core implementation
- **Kraken API**: Market data and order execution
- **cpr**: HTTP requests
- **nlohmann/json**: JSON parsing
- **OpenSSL**: HMAC and SHA256 authentication
- **Multithreading**: For real-time data processing

## Project Structure
/src
	- |Transaction.h/.cpp # Handles trade logging and transaction records.
	- |Trade.h/.cpp # Manages API authentication, order execution, and balance status.
	- |TechAnal.h/.cpp # Implements technical analysis (EMA, RSI, Stochastic).
	- |Strategy.h/.cpp # Determines buy/sell signals based on market conditions.
	- |main.cpp # Entry point, integrates all modules.

	
## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/0-0017/Trade
   cd Trade/projectScalar
2. Install dependencies (for Windows, ensure OpenSSL and cURL are installed)
3. Compile the project:
	```sh
	g++ -o Trade main.cpp Transaction.cpp Trade.cpp TechAnal.cpp Strategy.cpp -lcurl -lssl -lcrypto -lpthread
4. Run the bot:
	```sh
	./Trade

## Usage
	- Configure API keys in Trade.cpp (replace placeholders).

	- Run the bot, and it will analyze market conditions and execute trades accordingly.

	- Modify Strategy.cpp to adjust trading parameters.

## Future Enhancements
	- Implement WebSocket for real-time market data.

	- Add backtesting for strategy evaluation.

	- Improve risk management with stop-loss and take-profit refinements.
