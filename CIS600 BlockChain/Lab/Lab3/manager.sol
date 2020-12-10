pragma solidity ^0.4.13;

contract Escrow {
  function free() public returns (bool) { }
  function newTx(address, address, string, uint256) public { }
}

contract SimpleToken {
  function getBalance(address) public returns (uint256) { }
}

contract Manager {

  struct Product {
    string productName;
    uint256 productPrice;
  }

  struct Shop {
    address sellerAddr;
    mapping (string => Product) productList;
  }

  mapping (address => Shop) accounts;

  Escrow escrow;

  SimpleToken tokenContract;

  function Manager(address escrowAddr, address tokenContractAddr) public {
    escrow = Escrow(escrowAddr);
    tokenContract = SimpleToken(tokenContractAddr);
  }

  function addNewProduct(string productName, uint256 productPrice) public returns (bool) {
    accounts[msg.sender].sellerAddr = msg.sender;
    accounts[msg.sender].productList[productName].productName = productName;
    accounts[msg.sender].productList[productName].productPrice = productPrice;
  }

  function newTransaction(address buyer, address seller, string productName) public returns (bool) {
    require(accounts[seller].productList[productName].productPrice > 0); // Valid Product
    require(escrow.free()); // Check if escrow contract is busy
    escrow.newTx(buyer, seller, productName, accounts[seller].productList[productName].productPrice);
    return true;
  }
  
  function getPrice(address seller, string productName) public returns (uint256) {
    return accounts[seller].productList[productName].productPrice;
  }
}
