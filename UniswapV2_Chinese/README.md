# UniswapV2

### **对UniswapV2的代码进行了进一步中文注释，基本上是逐行注释，其中包含了许多关键字的解释和功能解释。如有需要请打开各自具体的sol文件进行查看，希望能帮助到您，如有任何错误或者建议希望能及时联系我**.

### 致谢：
kpyaoqi提供的uniswapV2代码：
https://github.com/kpyaoqi/UniswapV2_Chinese.git

成都信息工程学院梁培利老师的区块链金融公开课：
https://www.bilibili.com/video/BV1xs4y127xW


# 深入解析 Uniswap V2 核心功能

本项目通过分析 Uniswap V2 的核心合约代码，详细拆解了其三大核心功能：**添加流动性**、**代币交易**和**移除流动性**。每个功能都配有具体的例子和步骤，旨在帮助开发者和学习者更好地理解其工作原理。

---

## 1. 添加流动性（Adding Liquidity）

该功能允许流动性提供者（LP）向交易对池中注入等值的两种代币，从而获得流动性代币（LP Token），并以此赚取手续费。

### **具体例子**

假设一位 LP 想要为 **WETH-DAI** 交易对添加流动性，决定注入 **10 WETH** 和 **25000 DAI**。

### **实现步骤**

1.  **用户调用路由器合约**：
    * **合约**：`UniswapV2Router02.sol`
    * **函数**：`addLiquidity(address tokenA, address tokenB, uint amountADesired, uint amountBDesired, uint amountAMin, uint amountBMin, address to, uint deadline)`
    * **功能**：LP 调用此函数，传入希望添加的两种代币地址和数量。

2.  **路由器转账**：
    * **合约**：`UniswapV2Router02.sol`
    * **函数**：`TransferHelper.safeTransferFrom(tokenA, msg.sender, pair, amountA)` 和 `TransferHelper.safeTransferFrom(tokenB, msg.sender, pair, amountB)`
    * **功能**：路由器合约会调用这个外部库函数，将 **10 WETH** 和 **25000 DAI** 从 LP 的钱包地址转移到相应的 **`WETH-DAI` 交易对合约**。

3.  **交易对合约铸造 LP Token**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`mint(address to)`
    * **核心逻辑**：
        * 交易对合约首先计算池子当前储备量 `balance0` 和 `balance1`。
        * 然后根据恒定乘积公式和接收到的代币数量，计算出应铸造的流动性数量 (`liquidity`)。
        * **关键代码**：`liquidity = Math.sqrt(amount0.mul(amount1));` （简化版，忽略了 `MINIMUM_LIQUIDITY`）

4.  **LP Token 发送**：
    * **合约**：`UniswapV2Pair.sol` 和 `UniswapV2ERC20.sol`
    * **函数**：`mint` 函数进一步调用 `UniswapV2ERC20.sol` 中的 `_mint(address to, uint value)` 函数。
    * **功能**：`_mint` 函数负责实际的代币铸造，它会增加总供应量 `totalSupply` 并更新 LP 地址的 `balanceOf`。

5.  **更新状态**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`_update(uint balance0, uint balance1, uint _reserve0, uint _reserve1)`
    * **事件**：`emit Mint(msg.sender, liquidity);` 和 `emit Sync(reserve0, reserve1);`
    * **功能**：更新内部的代币储备量并触发事件，记录本次操作。

---

## 2. 代币交易（Swapping Tokens）

该功能允许用户用池中的一种代币兑换另一种代币，交易过程完全在链上自动完成。

### **具体例子**

假设你想要用 **1000 DAI** 兑换 **WETH**，并设置了最低接收量为 **0.38 WETH**。

### **实现步骤**

1.  **用户调用路由器合约**：
    * **合约**：`UniswapV2Router02.sol`
    * **函数**：`swapExactTokensForTokens(uint amountIn, uint amountOutMin, address[] calldata path, address to, uint deadline)`
    * **功能**：你调用此函数，传入 1000 DAI、最低接收量和交易路径。

2.  **路由器转账**：
    * **合约**：`UniswapV2Router02.sol`
    * **函数**：`TransferHelper.safeTransferFrom(path[0], msg.sender, UniswapV2Library.pairFor(factory, path[0], path[1]), amountIn)`
    * **功能**：路由器合约调用此函数，将 **1000 DAI** 从你的钱包转移到 **DAI-WETH** 交易对合约。

3.  **交易对合约执行兑换**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`swap(uint amount0Out, uint amount1Out, address to, bytes calldata data)`
    * **核心逻辑**：
        * 交易对合约根据转入的 DAI 数量，利用恒定乘积公式计算应转出的 WETH 数量。
        * **关键代码**：`uint amount1Out = getAmountOut(amount0In, _reserve0, _reserve1);`

4.  **代币发送**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`_safeTransfer(address token, address to, uint value)`
    * **功能**：`swap` 函数调用此内部函数，将计算出的 WETH 从交易对合约转账到你的钱包地址。

5.  **更新状态**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`_update(balance0, balance1, _reserve0, _reserve1)`
    * **事件**：`emit Swap(msg.sender, amount0In, amount1In, amount0Out, amount1Out, to);`
    * **功能**：更新储备量并触发 `Swap` 事件。

---

## 3. 移除流动性（Removing Liquidity）

该功能允许 LP 赎回其 LP Token，并按比例取回两种基础代币。

### **具体例子**

假设你持有 **100 个 LP Token**，想要从 **WETH-DAI** 交易对中移除流动性。

### **实现步骤**

1.  **用户调用路由器合约**：
    * **合约**：`UniswapV2Router02.sol`
    * **函数**：`removeLiquidity(address tokenA, address tokenB, uint liquidity, uint amountAMin, uint amountBMin, address to, uint deadline)`
    * **功能**：你调用此函数，传入想要移除的 LP Token 数量。

2.  **路由器转账 LP Token**：
    * **合约**：`UniswapV2Router02.sol`
    * **函数**：`TransferHelper.safeTransferFrom(pair, msg.sender, pair, liquidity)`
    * **功能**：路由器合约将 **100 个 LP Token** 从你的钱包转移到 **`WETH-DAI` 交易对合约**。

3.  **交易对合约销毁 LP Token**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`burn(address to)`
    * **核心逻辑**：
        * `burn` 函数调用 `UniswapV2ERC20.sol` 中的 `_burn(address to, uint value)` 函数，销毁你转入的 LP Token。
        * 接着，根据 `liquidity` 占 `totalSupply` 的比例，计算出应返还的 WETH 和 DAI 数量。
        * **关键代码**：`uint amount0 = liquidity.mul(balance0) / _totalSupply;` 和 `uint amount1 = liquidity.mul(balance1) / _totalSupply;`

4.  **代币发送**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`_safeTransfer(token0, to, amount0)` 和 `_safeTransfer(token1, to, amount1)`
    * **功能**：`burn` 函数调用此内部函数，将计算出的 WETH 和 DAI 转账到你的钱包地址。

5.  **更新状态**：
    * **合约**：`UniswapV2Pair.sol`
    * **函数**：`_update(balance0, balance1, _reserve0, _reserve1)`
    * **事件**：`emit Burn(msg.sender, amount0, amount1, to);` 和 `emit Sync(reserve0, reserve1);`
    * **功能**：更新储备量并触发事件，记录移除流动性的操作。



