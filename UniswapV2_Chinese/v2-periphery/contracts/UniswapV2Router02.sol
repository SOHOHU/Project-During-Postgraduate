pragma solidity =0.6.6;

import '@uniswap/v2-core/contracts/interfaces/IUniswapV2Factory.sol';
import '@uniswap/lib/contracts/libraries/TransferHelper.sol';

import './interfaces/IUniswapV2Router02.sol';
import './libraries/UniswapV2Library.sol';
import './libraries/SafeMath.sol';
import './interfaces/IERC20.sol';
import './interfaces/IWETH.sol';

contract UniswapV2Router02 is IUniswapV2Router02 {
    using SafeMath for uint;
    // UniswapFactory合约地址
    // 关键字解析：
    // 1、immutable：表示这个变量只能在构造函数中被赋值一次
    // 2、override：表示这个变量覆盖了父类中的同名变量
    address public immutable override factory;
    // WETH合约地址
    address public immutable override WETH;
    // 确保时间是有效的，即大于新区块产生时的时间戳
    modifier ensure(uint deadline) {
        require(deadline >= block.timestamp, 'UniswapV2Router: EXPIRED');
        _;
    }
    // 构造函数，之前在Factory中已经详细解释
    constructor(address _factory, address _WETH) public {
        factory = _factory;
        WETH = _WETH;
    }
    // 关键字解析：
    // 1、Solidity 中只有两种特殊的函数可以省略 function 关键字，它们是：receive()和fallback()这两个函数被称为回退函数
    // 没有名称，因此不能被直接调用。只有在特定的回退条件下才会被自动执行
    // 2、assert() 函数用于检查一个条件是否为真。如果条件为假，它会导致交易回滚并消耗所有剩余的 Gas
    receive() external payable {
        // 仅通过WETH合同的回退接受ETH
        // 确保只有 WETH 合约才能向此路由器合约直接发送 ETH，它防止了用户意外地向路由器合约发送 ETH
        assert(msg.sender == WETH);
    }
    // 补充解释：
    // ETH不完全遵循 ERC20 代币标准，而Uniswap V2 等许多智能合约和去中心化金融（DeFi）协议主要是为了处理符合 ERC20 标准的代币而设计的
    // 例如，一个典型的 ERC20 代币交易需要调用 approve 函数来授权合约，然后调用 transferFrom 来转移代币。但是，原生 ETH 没有 approve 和 transferFrom 函数
    // WETH 合约将 ETH 封装为 ERC20 代币 能够在交易所进行交易
    // 其基本原理为
        // 1、封装 ETH: 你可以向 WETH 合约发送 ETH。每发送 1 个 ETH，WETH 合约就会为你铸造 1 个 WETH 代币。这个过程在技术上称为 deposit（存款）。
        // 2、解封 ETH: 你可以向 WETH 合约发送 WETH 代币。每发送 1 个 WETH，合约就会销毁该代币，并向你发送 1 个原生 ETH。这个过程称为 withdraw（取款）

    /**
     * @dev: 根据两种token的地址向其交易对添加流动性
     * @param {address} tokenA:tokenA地址
     * @param {address} tokenB:tokenB地址
     * @param {uint} amountADesired:期望添加tokenA的数量
     * @param {uint} amountBDesired:期望添加tokenB的数量
     * @param {uint} amountAMin:愿意接受的最低tokenA数量，用于控制滑点
     * @param {uint} amountBMin:愿意接受的最低tokenB数量，用于控制滑点
     * @return {uint} amountA:实际添加到资金池中tokenA的数量
     * @return {uint} amountB:实际添加到资金池中tokenB的数量
     */
    // 计算最佳流动性值
    // 关键字解析：
    // 1、internal:表示这个函数只能在当前合约或派生合约中调用，不能通过外部交易或其他合约直接调用
    // 2、virtual:表示这个函数可以被子合约重写（覆盖）。与override不同在于，override是正在覆盖。
    // 不过有趣的是，solidity默认接口的virtual了
    function _addLiquidity(
        address tokenA,
        address tokenB,
        uint amountADesired,
        uint amountBDesired,
        uint amountAMin,
        uint amountBMin
    ) internal virtual returns (uint amountA, uint amountB) {
        // 拿到lpToken的地址，若不存在则创建一个交易对
        // 这里涉及address到接口的强制转换，在pair中已经说过
        if (IUniswapV2Factory(factory).getPair(tokenA, tokenB) == address(0)) {
            IUniswapV2Factory(factory).createPair(tokenA, tokenB);
        }
        // 获取两种token的储备量
        (uint reserveA, uint reserveB) = UniswapV2Library.getReserves(factory, tokenA, tokenB);
        if (reserveA == 0 && reserveB == 0) {
            // 首次添加流动性，实际添加的直接就是用户期望添加的
            (amountA, amountB) = (amountADesired, amountBDesired);
        } else {
            // 根据两种token的储备量和期望tokenA的数额获取tokenB最佳的数额
            // 说明uniswapV2并不会根据用户想加多少就加多少，因为要控制滑点，会根据实际情况找到一个最佳值（即维持现有比例）
            // 功能解析：
            // 参考reserveA, reserveB的比例，以amountADesired为基准。推出amountBOptimal和amountAOptimal的应有比例
            uint amountBOptimal = UniswapV2Library.quote(amountADesired, reserveA, reserveB);
            // 如果amountBOptimal不大于amountBDesired并且amountBOptimal不小于amountBMin，则返回amountADesired, amountBOptimal
            if (amountBOptimal <= amountBDesired) {
                require(amountBOptimal >= amountBMin, 'UniswapV2Router: INSUFFICIENT_B_AMOUNT');
                (amountA, amountB) = (amountADesired, amountBOptimal);
            } else {
                // 如果amountBOptimal大于amountBDesired，则根据两种token的储备量和期望tokenB的数额获取tokenA最佳的数额
                // 简单来说，如果以A为基准B算出来不符合要求，就以B为基准计算A
                uint amountAOptimal = UniswapV2Library.quote(amountBDesired, reserveB, reserveA);
                // 断言
                assert(amountAOptimal <= amountADesired);
                // 并且amountAOptimal不小于amountAMin，则返回amountAOptimal, amountBDesired
                require(amountAOptimal >= amountAMin, 'UniswapV2Router: INSUFFICIENT_A_AMOUNT');
                (amountA, amountB) = (amountAOptimal, amountBDesired);
            }
        }
    }

    /**
     * @dev: 根据两种token的地址向其交易对添加流动性
     * @param {address} tokenA:tokenA地址
     * @param {address} tokenB:tokenB地址
     * @param {uint} amountADesired:期望添加tokenA的数量
     * @param {uint} amountBDesired:期望添加tokenB的数量
     * @param {uint} amountAMin:愿意接受的最低tokenA数量
     * @param {uint} amountBMin:愿意接受的最低tokenB数量
     * @param {address} to:接受lptoken的地址
     * @param {uint} deadline:交易允许最后执行时间
     * @return {uint} amountA:实际添加到资金池中tokenA的数量
     * @return {uint} amountB:实际添加到资金池中tokenB的数量
     * @return {uint} liquidity:获得lptoken的数量
     */
    function addLiquidity(
        address tokenA,
        address tokenB,
        uint amountADesired,
        uint amountBDesired,
        uint amountAMin,
        uint amountBMin,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) returns (uint amountA, uint amountB, uint liquidity) {
        // internal关键字的体现
        (amountA, amountB) = _addLiquidity(tokenA, tokenB, amountADesired, amountBDesired, amountAMin, amountBMin);
        address pair = UniswapV2Library.pairFor(factory, tokenA, tokenB);
        // 转账两种token的amount数量到pair合约
        // 根据ERC20的标准（详见Core/constract/ERC20）合约无权访问用户的资产，必须先授权，再转账
        TransferHelper.safeTransferFrom(tokenA, msg.sender, pair, amountA);
        TransferHelper.safeTransferFrom(tokenB, msg.sender, pair, amountB);
        // 向to地址铸造lptoken，铸造的主体还是pair哦
        liquidity = IUniswapV2Pair(pair).mint(to);
    }

    /**
     * @dev: 根据token的地址向其与WETH合约的交易对添加流动性
     * @param {address} token:token地址
     * @param {uint} amountDesired:期望添加token的数量
     * @param {uint} amountMin:愿意接受的最低token数量
     * @param {address} to:接受lptoken的地址
     * @param {uint} deadline:交易允许最后执行时间
     * @return {uint} amountToken:实际添加到资金池中token的数量
     * @return {uint} amountETH:实际添加到资金池中ETH的数量
     * @return {uint} liquidity:获得lptoken的数量
     */
    // 考虑到ETH的特殊性，专门写了一个函数交互WETH
    function addLiquidityETH(
        address token,
        uint amountTokenDesired,
        uint amountTokenMin,
        uint amountETHMin,
        address to,
        uint deadline
    ) external payable virtual override ensure(deadline) returns (uint amountToken, uint amountETH, uint liquidity) {
        (amountToken, amountETH) = _addLiquidity(
            token,
            WETH,
            amountTokenDesired,
            msg.value,
            amountTokenMin,
            amountETHMin
        );
        address pair = UniswapV2Library.pairFor(factory, token, WETH);
        TransferHelper.safeTransferFrom(token, msg.sender, pair, amountToken);
        //  ETH 的封装操作。这一步会将计算出的 amountETH 数量的 ETH 发送给 WETH 合约，从而铸造出相应数量的 WETH 代币。
        IWETH(WETH).deposit{value: amountETH}();
        assert(IWETH(WETH).transfer(pair, amountETH));
        liquidity = IUniswapV2Pair(pair).mint(to);
        // 如果转入ETH有多余的，退还
        if (msg.value > amountETH) TransferHelper.safeTransferETH(msg.sender, msg.value - amountETH);
    }

    /**
     * @dev: 根据两种token的地址向其交易对移除流动性
     * @param {address} tokenA:tokenA地址
     * @param {address} tokenB:tokenB地址
     * @param {uint} liquidity:移除lptoken的数量
     * @param {uint} amountAMin:愿意接受的最低tokenA数量，用于控制滑点
     * @param {uint} amountBMin:愿意接受的最低tokenB数量，用于控制滑点
     * @param {address} to:接受两种token的地址
     * @param {uint} deadline:交易允许最后执行时间
     * @return {uint} amountA:移除流动性获得tokenA的数量
     * @return {uint} amountB:移除流动性获得tokenB的数量
     */
    // 移除流动性，比较简单
    function removeLiquidity(
        address tokenA,
        address tokenB,
        uint liquidity,
        uint amountAMin,
        uint amountBMin,
        address to,
        uint deadline
    ) public virtual override ensure(deadline) returns (uint amountA, uint amountB) {
        address pair = UniswapV2Library.pairFor(factory, tokenA, tokenB);
        // 将lptoken发送到pair合约
        IUniswapV2Pair(pair).transferFrom(msg.sender, pair, liquidity);
        // 销毁lptoken，返回销毁lptoken获得两种token的数量
        (uint amount0, uint amount1) = IUniswapV2Pair(pair).burn(to);
        (address token0, ) = UniswapV2Library.sortTokens(tokenA, tokenB);
        (amountA, amountB) = tokenA == token0 ? (amount0, amount1) : (amount1, amount0);
        // 判断返回数量是否大于所设置的最小返回值
        require(amountA >= amountAMin, 'UniswapV2Router: INSUFFICIENT_A_AMOUNT');
        require(amountB >= amountBMin, 'UniswapV2Router: INSUFFICIENT_B_AMOUNT');
    }

    function removeLiquidityETH(
        address token,
        uint liquidity,
        uint amountTokenMin,
        uint amountETHMin,
        address to,
        uint deadline
    ) public virtual override ensure(deadline) returns (uint amountToken, uint amountETH) {
        // 先将返回的token和ETH发送到当前合约
        (amountToken, amountETH) = removeLiquidity(
            token,
            WETH,
            liquidity,
            amountTokenMin,
            amountETHMin,
            address(this),
            deadline
        );
        // 转到to地址
        TransferHelper.safeTransfer(token, to, amountToken);
        IWETH(WETH).withdraw(amountETH);
        TransferHelper.safeTransferETH(to, amountETH);
    }

    /**
     * @dev: 相对于removeLiquidity引入了许可功能进行身份验证
     * @param ......
     * @param {bool} approveMax:用于指示是否在移除流动性之前使用permit功能进行身份验证
     * @param {uint8} v:原始签名的v
     * @param {bytes32} r:原始签名的r
     * @param {bytes32} s:原始签名的s
     * @return ......
     */
    // 到这里才是external，真正外部可用的。removeLiquidity仅展示核心功能，展示了程序设计的复用性
    function removeLiquidityWithPermit(
        address tokenA,
        address tokenB,
        uint liquidity,
        uint amountAMin,
        uint amountBMin,
        address to,
        uint deadline,
        bool approveMax,
        uint8 v,
        bytes32 r,
        bytes32 s
    ) external virtual override returns (uint amountA, uint amountB) {
        address pair = UniswapV2Library.pairFor(factory, tokenA, tokenB);
        uint value = approveMax ? uint(-1) : liquidity;
        // 身份验证
        // 在pair中已经详细解释过
        IUniswapV2Pair(pair).permit(msg.sender, address(this), value, deadline, v, r, s);
        (amountA, amountB) = removeLiquidity(tokenA, tokenB, liquidity, amountAMin, amountBMin, to, deadline);
    }

    function removeLiquidityETHWithPermit(
        address token,
        uint liquidity,
        uint amountTokenMin,
        uint amountETHMin,
        address to,
        uint deadline,
        bool approveMax,
        uint8 v,
        bytes32 r,
        bytes32 s
    ) external virtual override returns (uint amountToken, uint amountETH) {
        address pair = UniswapV2Library.pairFor(factory, token, WETH);
        uint value = approveMax ? uint(-1) : liquidity;
        IUniswapV2Pair(pair).permit(msg.sender, address(this), value, deadline, v, r, s);
        (amountToken, amountETH) = removeLiquidityETH(token, liquidity, amountTokenMin, amountETHMin, to, deadline);
    }

    // 移除流动性（支持转账手续费代币）
    /**
     * @dev: 相对于removeLiquidityETH适用于处理在资金池中具有“费用分摊”（fee-on-transfer）机制的代币
     * @param ......
     * @return ......
     */
    // 用于有手续费的特殊情况，情况下ETH会被收取手续费，使用这种操作可以避免可能的require检查失败
    function removeLiquidityETHSupportingFeeOnTransferTokens(
        address token,
        uint liquidity,
        uint amountTokenMin,
        uint amountETHMin,
        address to,
        uint deadline
    ) public virtual override ensure(deadline) returns (uint amountETH) {
        (, amountETH) = removeLiquidity(token, WETH, liquidity, amountTokenMin, amountETHMin, address(this), deadline);
        // 解决方法是将交易后的ETH和代币先存在路由合约内部，需要的时候再调用转账函数
        TransferHelper.safeTransfer(token, to, IERC20(token).balanceOf(address(this)));
        IWETH(WETH).withdraw(amountETH);
        TransferHelper.safeTransferETH(to, amountETH);
    }

    function removeLiquidityETHWithPermitSupportingFeeOnTransferTokens(
        address token,
        uint liquidity,
        uint amountTokenMin,
        uint amountETHMin,
        address to,
        uint deadline,
        bool approveMax,
        uint8 v,
        bytes32 r,
        bytes32 s
    ) external virtual override returns (uint amountETH) {
        address pair = UniswapV2Library.pairFor(factory, token, WETH);
        uint value = approveMax ? uint(-1) : liquidity;
        IUniswapV2Pair(pair).permit(msg.sender, address(this), value, deadline, v, r, s);
        amountETH = removeLiquidityETHSupportingFeeOnTransferTokens(
            token,
            liquidity,
            amountTokenMin,
            amountETHMin,
            to,
            deadline
        );
    }

    // 兑换，其实是执行了多跳兑换
    // 要求初始金额已经发送到第一对
    /**
     * @dev: 根据path路径和其amounts量进行交易对兑换
     * @param {uint[]} amounts:在每对交易对进行输入的token的数量，对应path
     * @param {address[]} path:当没有两种token的交易对，需要进行多个兑换(tokenA->tokenB->ETH)
     * @param {address} _to:接受兑换token的地址
     */
    function _swap(uint[] memory amounts, address[] memory path, address _to) internal virtual {
        // 循环path路径
        for (uint i; i < path.length - 1; i++) {
            // 计算每对交易对的兑换量，下面的逻辑比较简单
            (address input, address output) = (path[i], path[i + 1]);
            (address token0, ) = UniswapV2Library.sortTokens(input, output);
            // A换B，A的输出可不就是B的量嘛
            uint amountOut = amounts[i + 1];
            (uint amount0Out, uint amount1Out) = input == token0 ? (uint(0), amountOut) : (amountOut, uint(0));
            // 如果中间还有其他的路径（i < path.length - 2 成立），to地址为其中交易对（i+1和i+2）的pair地址
            address to = i < path.length - 2 ? UniswapV2Library.pairFor(factory, output, path[i + 2]) : _to;
            // 进行兑换
            IUniswapV2Pair(UniswapV2Library.pairFor(factory, input, output)).swap(
                amount0Out,
                amount1Out,
                to,
                new bytes(0)
            );
        }
    }

    /**
     * @dev: 根据确切的tokenA的数量兑换tokenB
     * @param {uint} amountIn:进行兑换的tokenA的数量
     * @param {uint} amountOutMin:愿意接受兑换后的最低tokenB数量，用于控制滑点
     * @param {address[]} path:进行兑换的路径
     * @param {address} to:接受兑换后获得tokenB的地址
     * @param {uint} deadline:交易允许最后执行时间
     * @return {uint[]} amounts:根据path路径获得每对交易对获得的token，最后一个为获得兑换后tokenB的数量
     */
    function swapExactTokensForTokens(
        uint amountIn,
        uint amountOutMin,
        address[] calldata path,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) returns (uint[] memory amounts) {
        // 获取兑换路径上每一步的输出数量
        amounts = UniswapV2Library.getAmountsOut(factory, amountIn, path);
        // 判断最终获得的tokenB的数量是否大于amountOutMin
        require(amounts[amounts.length - 1] >= amountOutMin, 'UniswapV2Router: INSUFFICIENT_OUTPUT_AMOUNT');
        // 将tokenA传入第一对交易对中，先完成第一对交易
        TransferHelper.safeTransferFrom(
            path[0],
            msg.sender,
            UniswapV2Library.pairFor(factory, path[0], path[1]),
            amounts[0]
        );
        // 保证第一个交易对有钱之后，才可以开始_swap
        _swap(amounts, path, to);
    }

    /**
     * @dev: 根据需要获得确切数量的tokenB传入需要tokenA的数量
     * @param {uint} amountOut:需要兑换后获得tokenB的数量
     * @param {uint} amountInMax:愿意接受兑换后的最高tokenA数量，用于控制滑点
     * @param ......
     * @return ......
     */
    // 反向操作，已知我想要的输出，求应该投入的输入
    function swapTokensForExactTokens(
        uint amountOut,
        uint amountInMax,
        address[] calldata path,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) returns (uint[] memory amounts) {
        amounts = UniswapV2Library.getAmountsIn(factory, amountOut, path);
        require(amounts[0] <= amountInMax, 'UniswapV2Router: EXCESSIVE_INPUT_AMOUNT');
        TransferHelper.safeTransferFrom(
            path[0],
            msg.sender,
            UniswapV2Library.pairFor(factory, path[0], path[1]),
            amounts[0]
        );
        _swap(amounts, path, to);
    }


    // 后面是两个为ETH专门写的函数
    // 根据确切数量的ETH的兑换token
    // (ETH -> Token) 发送确切数量的 ETH 获得不低于 amountOutMin
    function swapExactETHForTokens(
        uint amountOutMin,
        address[] calldata path,
        address to,
        uint deadline
    ) external payable virtual override ensure(deadline) returns (uint[] memory amounts) {
        require(path[0] == WETH, 'UniswapV2Router: INVALID_PATH');
        amounts = UniswapV2Library.getAmountsOut(factory, msg.value, path);
        require(amounts[amounts.length - 1] >= amountOutMin, 'UniswapV2Router: INSUFFICIENT_OUTPUT_AMOUNT');
        IWETH(WETH).deposit{value: amounts[0]}();
        assert(IWETH(WETH).transfer(UniswapV2Library.pairFor(factory, path[0], path[1]), amounts[0]));
        _swap(amounts, path, to);
    }

    // 根据需要获得确切数量的ETH传入需要token的数量
    // (Token -> ETH) 发送不超过 amountInMax 的代币 获得确切数量的 ETH
    function swapTokensForExactETH(
        uint amountOut,
        uint amountInMax,
        address[] calldata path,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) returns (uint[] memory amounts) {
        require(path[path.length - 1] == WETH, 'UniswapV2Router: INVALID_PATH');
        amounts = UniswapV2Library.getAmountsIn(factory, amountOut, path);
        require(amounts[0] <= amountInMax, 'UniswapV2Router: EXCESSIVE_INPUT_AMOUNT');
        TransferHelper.safeTransferFrom(
            path[0],
            msg.sender,
            UniswapV2Library.pairFor(factory, path[0], path[1]),
            amounts[0]
        );
        _swap(amounts, path, address(this));
        IWETH(WETH).withdraw(amounts[amounts.length - 1]);
        TransferHelper.safeTransferETH(to, amounts[amounts.length - 1]);
    }

    // 根据确切数量的token的兑换ETH
    // (Token -> ETH)  用户提供确切数量的代币  获得不低于 amountOutMin 的 ETH
    function swapExactTokensForETH(
        uint amountIn,
        uint amountOutMin,
        address[] calldata path,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) returns (uint[] memory amounts) {
        require(path[path.length - 1] == WETH, 'UniswapV2Router: INVALID_PATH');
        amounts = UniswapV2Library.getAmountsOut(factory, amountIn, path);
        require(amounts[amounts.length - 1] >= amountOutMin, 'UniswapV2Router: INSUFFICIENT_OUTPUT_AMOUNT');
        TransferHelper.safeTransferFrom(
            path[0],
            msg.sender,
            UniswapV2Library.pairFor(factory, path[0], path[1]),
            amounts[0]
        );
        _swap(amounts, path, address(this));
        IWETH(WETH).withdraw(amounts[amounts.length - 1]);
        TransferHelper.safeTransferETH(to, amounts[amounts.length - 1]);
    }

    // 根据需要获得确切数量的token传入需要ETH的数量
    // (ETH -> Token) 用户发送最高不超过 msg.value 的 ETH 获得确切数量的代币 (amountOut)
    function swapETHForExactTokens(
        uint amountOut,
        address[] calldata path,
        address to,
        uint deadline
    ) external payable virtual override ensure(deadline) returns (uint[] memory amounts) {
        require(path[0] == WETH, 'UniswapV2Router: INVALID_PATH');
        amounts = UniswapV2Library.getAmountsIn(factory, amountOut, path);
        require(amounts[0] <= msg.value, 'UniswapV2Router: EXCESSIVE_INPUT_AMOUNT');
        IWETH(WETH).deposit{value: amounts[0]}();
        assert(IWETH(WETH).transfer(UniswapV2Library.pairFor(factory, path[0], path[1]), amounts[0]));
        _swap(amounts, path, to);
        // refund dust eth, if any
        if (msg.value > amounts[0]) TransferHelper.safeTransferETH(msg.sender, msg.value - amounts[0]);
    }

    // 兑换（支持转账收费令牌）,用于解决部分ERC20交易的时候生成手续费导致无法require的情况
    // route02新增
    // 要求初始金额已经发送到第一对
    function _swapSupportingFeeOnTransferTokens(address[] memory path, address _to) internal virtual {
        // 循环path路径
        for (uint i; i < path.length - 1; i++) {
            (address input, address output) = (path[i], path[i + 1]);
            (address token0, ) = UniswapV2Library.sortTokens(input, output);
            IUniswapV2Pair pair = IUniswapV2Pair(UniswapV2Library.pairFor(factory, input, output));
            uint amountInput;
            uint amountOutput;
            {
                // 避免堆栈太深的错误
                // 获取交易对两种token的储备对
                (uint reserve0, uint reserve1, ) = pair.getReserves();
                (uint reserveInput, uint reserveOutput) = input == token0 ? (reserve0, reserve1) : (reserve1, reserve0);
                // 获取用户传入tokenA的数量
                // 核心，也是它和_swap的本质区别：功能解析：
                // 1、通过监听pair中token存量的变化来判断用户传入了多少Token，再进行转换和手续费等计算直接得到用户赢得的TokenB
                // 2、_swap中是用户给定了amounts，基于amounts进行计算。但是swap因为“用空间换时间”，比较省gas，是uniswap中常见的代码
                amountInput = IERC20(input).balanceOf(address(pair)).sub(reserveInput);
                // 根据用户传入tokenA的数量获取另一token的数量
                amountOutput = UniswapV2Library.getAmountOut(amountInput, reserveInput, reserveOutput);
            }
            (uint amount0Out, uint amount1Out) = input == token0 ? (uint(0), amountOutput) : (amountOutput, uint(0));
            address to = i < path.length - 2 ? UniswapV2Library.pairFor(factory, output, path[i + 2]) : _to;
            pair.swap(amount0Out, amount1Out, to, new bytes(0));
        }
    }

    // 基于_swapSupportingFeeOnTransferTokens，同样也可以实现交易
    // 但是uniswap认为在有手续费的情况下，确定tokenB（输出）来倒推tokenA（小于amountInMax）是不安全的，如swapTokensForExactETH。因此所有的交易只现定于确定输入推输出（确保大于amountOutMin）
    // 根据确切的tokenA的数量兑换tokenB
    function swapExactTokensForTokensSupportingFeeOnTransferTokens(
        uint amountIn,
        uint amountOutMin,
        address[] calldata path,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) {
        // 将tokenA传入pair合约地址，还是老生常谈的ERC20规范
        TransferHelper.safeTransferFrom(
            path[0],
            msg.sender,
            UniswapV2Library.pairFor(factory, path[0], path[1]),
            amountIn
        );
        // 兑换前to地址的tokenB的余额
        uint balanceBefore = IERC20(path[path.length - 1]).balanceOf(to);
        // 兑换，已知地址就可以直接进行兑换
        _swapSupportingFeeOnTransferTokens(path, to);
        // 兑换后to地址的tokenB的余额-balanceBefore需要 >= amountOutMin
        require(
            IERC20(path[path.length - 1]).balanceOf(to).sub(balanceBefore) >= amountOutMin,
            'UniswapV2Router: INSUFFICIENT_OUTPUT_AMOUNT'
        );
    }

    // 关于ETH的正反向操作
    function swapExactETHForTokensSupportingFeeOnTransferTokens(
        uint amountOutMin,
        address[] calldata path,
        address to,
        uint deadline
    ) external payable virtual override ensure(deadline) {
        require(path[0] == WETH, 'UniswapV2Router: INVALID_PATH');
        uint amountIn = msg.value;
        IWETH(WETH).deposit{value: amountIn}();
        assert(IWETH(WETH).transfer(UniswapV2Library.pairFor(factory, path[0], path[1]), amountIn));
        uint balanceBefore = IERC20(path[path.length - 1]).balanceOf(to);
        _swapSupportingFeeOnTransferTokens(path, to);
        require(
            IERC20(path[path.length - 1]).balanceOf(to).sub(balanceBefore) >= amountOutMin,
            'UniswapV2Router: INSUFFICIENT_OUTPUT_AMOUNT'
        );
    }

    function swapExactTokensForETHSupportingFeeOnTransferTokens(
        uint amountIn,
        uint amountOutMin,
        address[] calldata path,
        address to,
        uint deadline
    ) external virtual override ensure(deadline) {
        require(path[path.length - 1] == WETH, 'UniswapV2Router: INVALID_PATH');
        TransferHelper.safeTransferFrom(
            path[0],
            msg.sender,
            UniswapV2Library.pairFor(factory, path[0], path[1]),
            amountIn
        );
        _swapSupportingFeeOnTransferTokens(path, address(this));
        uint amountOut = IERC20(WETH).balanceOf(address(this));
        require(amountOut >= amountOutMin, 'UniswapV2Router: INSUFFICIENT_OUTPUT_AMOUNT');
        IWETH(WETH).withdraw(amountOut);
        TransferHelper.safeTransferETH(to, amountOut);
    }

    // 库函数
    // 用于在给定一种代币的输入数量和两种代币的储备量时，计算出另一种代币的最优输出数量
    function quote(uint amountA, uint reserveA, uint reserveB) public pure virtual override returns (uint amountB) {
        return UniswapV2Library.quote(amountA, reserveA, reserveB);
    }

    // 函数用于计算在给定输入代币数量的情况下，可以获得多少输出代币数量
    function getAmountOut(
        uint amountIn,
        uint reserveIn,
        uint reserveOut
    ) public pure virtual override returns (uint amountOut) {
        return UniswapV2Library.getAmountOut(amountIn, reserveIn, reserveOut);
    }

    // 函数用于计算在给定输出代币数量的情况下，需要多少输入代币数量
    function getAmountIn(
        uint amountOut,
        uint reserveIn,
        uint reserveOut
    ) public pure virtual override returns (uint amountIn) {
        return UniswapV2Library.getAmountIn(amountOut, reserveIn, reserveOut);
    }

    // 函数用于计算沿着一个交易路径（path）进行兑换时，每个步骤的输出金额
    function getAmountsOut(
        uint amountIn,
        address[] memory path
    ) public view virtual override returns (uint[] memory amounts) {
        return UniswapV2Library.getAmountsOut(factory, amountIn, path);
    }

    // getAmountsIn 函数与 getAmountsOut 相反，用于计算为了在给定的交易路径上获得特定数量的最终代币，每个步骤所需的输入金额 
    function getAmountsIn(
        uint amountOut,
        address[] memory path
    ) public view virtual override returns (uint[] memory amounts) {
        return UniswapV2Library.getAmountsIn(factory, amountOut, path);
    }
}
