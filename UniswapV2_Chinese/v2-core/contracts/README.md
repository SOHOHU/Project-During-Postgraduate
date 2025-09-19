# UniswapV2 核心合约

这个 `core` 目录包含了 Uniswap V2 协议的核心智能合约。它们定义了去中心化流动性池和自动做市商（AMM）的基础功能。

## UniswapV2Factory

[cite_start]在构造函数中传入一个设置 `feeTo` 的权限者地址，主要用于创建两种代币的交易对，并为每个交易对部署一个 `UniswapV2Pair` 合约进行管理 [cite: 30, 31][cite_start]。此外，`UniswapV2Factory` 也包含了一些与手续费相关的设置功能 [cite: 31]。

### 主要方法

* [cite_start]`feeTo()`：返回收取手续费的地址 [cite: 31]。
* [cite_start]`feeToSetter()`：返回有权设置手续费地址的权限地址 [cite: 32]。
* [cite_start]`getPair(address tokenA, address tokenB)`：获取两种代币的交易对地址 [cite: 33]。
* [cite_start]`allPairs(uint)`：返回指定索引位置的交易对地址 [cite: 34]。
* [cite_start]`allPairsLength()`：返回所有已创建交易对的总数 [cite: 37]。
* [cite_start]`createPair(address tokenA, address tokenB)`：创建两种代币的交易对并返回其地址 [cite: 38]。
* [cite_start]`setFeeTo(address)`：更改收取手续费的地址 [cite: 54]。
* [cite_start]`setFeeToSetter(address)`：更改设置手续费地址的权限者地址 [cite: 55]。

---

## UniswapV2Pair

[cite_start]此合约由 `UniswapV2Factory` 部署，继承了 `UniswapV2ERC20`，主要用于管理和操作特定的交易对，并托管两种代币 [cite: 57][cite_start]。它在构造函数中将 `factory` 地址设置为部署者的地址 [cite: 72]。

### 重要方法

* [cite_start]`permit(address owner, address spender, uint value, uint deadline, uint8 v, bytes32 r, bytes32 s)`: **验证签名有效性** [cite: 24, 25][cite_start]。用户可以在链下对一笔授权进行签名，此方法通过 `ecrecover` 函数验证签名的有效性 [cite: 27, 28][cite_start]。如果验证通过，将进行授权 [cite: 29]。
* [cite_start]`mint(address to)`: **铸造 LP (流动性提供者) 代币** [cite: 91][cite_start]。此方法根据用户存入的两种代币数量，计算并铸造相应的 LP 代币给指定地址 [cite: 93, 99, 100][cite_start]。该函数使用了**防重入锁（`lock`）** [cite: 64, 65]。
* [cite_start]`burn(address to)`: **销毁 LP 代币并退出流动性** [cite: 104][cite_start]。用户首先将 LP 代币转移到 Pair 合约中，然后此函数按比例计算应返还的两种代币数量 [cite: 109][cite_start]，销毁 LP 代币 [cite: 111][cite_start]，并将代币转账给指定地址 [cite: 112]。
* [cite_start]`swap(uint amount0Out, uint amount1Out, address to, bytes calldata data)`: **执行代币兑换** [cite: 116][cite_start]。用户将需要兑换的代币转入 Pair 合约，此方法计算应输出的代币数量并进行转账 [cite: 120, 121][cite_start]。如果 `data` 参数不为空，还会执行回调函数，这使得**闪电贷**成为可能 [cite: 122]。
* [cite_start]`skim(address to)`: 确保代币余额与储备量一致 [cite: 130][cite_start]。此方法会将多余的代币（如果存在）转回指定地址 [cite: 130, 131]。
* [cite_start]`sync()`: 将代币储备量与合约的实际余额进行同步 [cite: 132]。
* [cite_start]`initialize(address, address)`: 由工厂合约在部署时调用一次，用于设置交易对中的两种代币地址 [cite: 73, 74]。

### 内部方法

* [cite_start]`_update(uint balance0, uint balance1, uint112 _reserve0, uint112 _reserve1)`: **更新储备和价格累计器** [cite: 75][cite_start]。它将合约的实际代币余额同步为新的储备量 [cite: 80][cite_start]，并根据时间间隔更新两种代币的价格累计值 [cite: 78, 79]。
* [cite_start]`_mintFee(uint112 _reserve0, uint112 _reserve1)`: **计算并铸造协议手续费** [cite: 83][cite_start]。此方法会在用户增加或移除流动性时被调用 [cite: 88, 89][cite_start]。如果协议手续费功能已启用，并且流动性池的 `k` 值有所增长 [cite: 85, 87][cite_start]，它会计算应收取的协议手续费 [cite: 90][cite_start]，并铸造新的 LP 代币给 `feeTo` 地址 [cite: 90]。
