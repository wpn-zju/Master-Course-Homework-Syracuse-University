pragma solidity ^ 0.4.13;
contract max {
    int result;

    function maximum(int x1, int x2) public {
        if (x1 >= x2) 
            result = x1;
        else
            result = x2;
    } 
       
    function getResult() public constant returns (int) {
        return result;
    }
}
