pragma solidity ^ 0.4.13;
contract rps {
    mapping (string => mapping(string => int)) payoffMatrix;
    
    address player1;
    address player2;
    
    string public player1Choice;
    string public player2Choice;

    modifier checkNotRegistered() {
        if (msg.sender == player1 || msg.sender == player2)
            revert();
        else
            _;
    }
    
    modifier sentEnoughCash(uint amount) {
        if (msg.value < amount)
            revert();
        else
            _;
    }
    
    // constructor
    function rps() {
        payoffMatrix["rock"]["rock"] = 0;
        payoffMatrix["rock"]["paper"] = 2;
        payoffMatrix["rock"]["scissors"] = 1;
        payoffMatrix["paper"]["rock"] = 1;
        payoffMatrix["paper"]["paper"] = 0;
        payoffMatrix["paper"]["scissors"] = 2;
        payoffMatrix["scissors"]["rock"] = 2;
        payoffMatrix["scissors"]["paper"] = 1;
        payoffMatrix["scissors"]["scissors"] = 0;
    }
    
    function getWinner() constant returns (int x) {
        return payoffMatrix[player1Choice][player2Choice];
    }
    
    function play(string choice) returns (int w) {
        if (msg.sender == player1)
            player1Choice = choice;
        else if (msg.sender == player2)
            player2Choice = choice;
        if (bytes(player1Choice).length != 0 && bytes(player2Choice).length != 0) {
            int winner = payoffMatrix[player1Choice][player2Choice];
            if (winner == 1)
                player1.transfer(this.balance);
            else if (winner == 2)
                player2.transfer(this.balance);
            else {
                player1.transfer(this.balance/2);
                player2.transfer(this.balance/2);
            }
            
            player1Choice = "";
            player2Choice = "";
            player1 = 0;
            player2 = 0;
            return winner;
        }
        else 
            return -1;
    }
    
    function getMyBalance() constant returns (uint amount) {
        return msg.sender.balance;
    }
    
    function getContractBalance() constant returns (uint amount) {
        return this.balance;
    }
    
    function register() payable
        sentEnoughCash(100)
        checkNotRegistered() {
        if (player1 == 0)
            player1 = msg.sender;
        else if (player2 == 0)
            player2 = msg.sender;
    }
    
    function AmIPlayer1() constant returns (bool x) {
        return msg.sender == player1;
    }
    
    function AmIPlayer2() constant returns (bool x) {
        return msg.sender == player2;
    }

    
    function checkBothNotNull() constant returns (bool x) {
        return bytes(player1Choice).length == 0 && bytes(player2Choice).length == 0;
    }
}
