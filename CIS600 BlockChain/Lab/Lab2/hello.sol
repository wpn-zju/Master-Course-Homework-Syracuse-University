pragma solidity ^ 0.4.13;
contract hello {
    string greeting;

    function greeter(string _greeting) public {
        greeting = _greeting;
    } 

    function greet() public constant returns(string) {
        return greeting;
    }
}
