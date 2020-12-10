pragma solidity ^0.4.13;

contract SimpleToken {
  mapping (address => uint256) balances;

  function getBalance(address addr) public returns (uint256) {
    return balances[addr];
  }

  function chargeSelf(uint amount) public returns (bool) {
    balances[msg.sender] = balances[msg.sender] + amount;
  }

  function transfer(address from, address to, uint256 amount) public returns (bool) {
    require(from != address(0));
    require(to != address(0));
    require(amount <= balances[from]);
    balances[from] = balances[from] - amount;
    balances[to] = balances[to] + amount;
    return true;
  }
}
