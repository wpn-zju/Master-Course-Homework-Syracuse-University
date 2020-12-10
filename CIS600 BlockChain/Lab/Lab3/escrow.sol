pragma solidity ^0.4.13;

contract SimpleToken {
  function getBalance(address) public returns (uint256) { }
  function transfer(address, address, uint256) public returns (bool) { }
}

contract Escrow { 
  SimpleToken tokenContract;

  address buyer;
  address seller;
  address arbitrator;

  bool public free = true;
  string productName;
  uint256 productPrice;

  bool buyerChecked;
  bool sellerChecked;

  string txState;
  string arbitratorDecision;

  uint startBlock;
  uint endBlock;

  function Escrow(address tokenContractAddr) public {
    tokenContract = SimpleToken(tokenContractAddr);
  }

  function newTx(address buy, address sell, string pName, uint256 pPrice) public {
    require (free == true);
    free = false;
    buyer = buy;
    seller = sell;
    productName = pName;
    productPrice = pPrice;
  }

  function MakeDeposit() {
    require (free == false);
    require (msg.sender == buyer);
    require (tokenContract.getBalance(msg.sender) >= productPrice);
    tokenContract.transfer(msg.sender, this, productPrice);
  }
  
  modifier sendEnoughCash() {
    if (msg.value < productPrice)
      revert();
    else
      _;
  }
  
  function MakeDepositEther() payable
      sendEnoughCash() {
    require (free == false);
    require (msg.sender == buyer);
  }

  function ApproveTxSuccess() {
    if (msg.sender == buyer) {
      buyerChecked = true;
    } else if (msg.sender == seller) {
      sellerChecked = true;
    }

    if ((!buyerChecked && sellerChecked) || (buyerChecked && !sellerChecked)) {
      startBlock = block.number;
      txState = "Dispute";
    }
  }

  function ApproveTxFail() {
    if (msg.sender == buyer) {
      buyerChecked = false;
    } else if (msg.sender == seller) {
      sellerChecked = false;
    }

    if ((!buyerChecked && sellerChecked) || (buyerChecked && !sellerChecked)) {
      startBlock = block.number;
      txState = "Dispute";
    }
  }

  function Arbitrate(string decision) {
    require(msg.sender == arbitrator);
    endBlock = block.number - startBlock;
    if (endBlock < 12 && compareStrings(txState, "Dispute")) {
      arbitratorDecision = decision;
    }
  }

  function transfer() public {
    if (buyerChecked == true && sellerChecked == true) {
      tokenContract.transfer(this, seller, productPrice);
    } else {
      if (buyerChecked == false && sellerChecked == false) {
        tokenContract.transfer(this, buyer, productPrice);
      } else {
        endBlock = block.number - startBlock;
        require(endBlock >= 12 || !compareStrings(arbitratorDecision, ""));
        if (endBlock >= 12 && compareStrings(arbitratorDecision, "")) {
          tokenContract.transfer(this, buyer, productPrice);
        } else {
          if (compareStrings(arbitratorDecision, "Success")) {
            tokenContract.transfer(this, seller, productPrice);
          } else if (compareStrings(arbitratorDecision, "Fail")) {
            tokenContract.transfer(this, buyer, productPrice);
          } else {
            tokenContract.transfer(this, buyer, productPrice);
          }
        }
      }
    }
    
    buyerChecked = false;
    sellerChecked = false;
    free = true;
    arbitratorDecision = "";
 }
 
 function transferEther() public {
    if (buyerChecked == true && sellerChecked == true) {
      seller.transfer(productPrice);
    } else {
      if (buyerChecked == false && sellerChecked == false) {
        buyer.transfer(productPrice);
      } else {
        endBlock = block.number - startBlock;
        require(endBlock >= 12 || !compareStrings(arbitratorDecision, ""));
        if (endBlock >= 12 && compareStrings(arbitratorDecision, "")) {
          buyer.transfer(productPrice);
        } else {
          if (compareStrings(arbitratorDecision, "Success")) {
            seller.transfer(productPrice);
          } else if (compareStrings(arbitratorDecision, "Fail")) {
            buyer.transfer(productPrice);
          } else {
            buyer.transfer(productPrice);
          }
        }
      }
    }
    
    buyerChecked = false;
    sellerChecked = false;
    free = true;
    arbitratorDecision = "";
 }

 function compareStrings(string s1, string s2) returns (bool) {
   return keccak256(s1) == keccak256(s2);
 }

  function setArbitrator() {
    arbitrator = msg.sender;
  }
}
