var crypto = require('crypto');
var Web3 = require('web3');
var web3 = new Web3(new Web3.providers.HttpProvider("http://127.0.0.1:7545"));

/* web3.eth.getBlockNumber(function(error, result){ if(!error) console.log(result) }) */
var defaultAcc = "";

setDefaultAccount();

function server() {
	var user = {
		'userA': 'pwd123',
		'userB': 'pwd456',
    };
    
    var fileX = "This is the file from server";
    
	var filePermissionBit = {
		'userA': 0,
		'userB': 0
    };
    
	var loginStatus = {
		'userA': 0,
		'userB': 0
	};

	this.user_Login = function (userId, pwd) {
        bkc_logging("user_login(\"" + userId + "\", \"" + pwd + "\")");
		if (user[userId] == pwd) {
			loginStatus[userId] = 1;
			event = userId + " Login";
		}
	}

	this.user_Logout = function (userId) {
		bkc_logging("user_logout(\"" + userId + "\")");
		loginStatus[userId] = 0;
	}

	this.file_permission_set = function (user) {
		bkc_logging("file_permission_set(\"" + user + "\")");
		filePermissionBit[user] = 1;
	}

	this.file_delegate = function (delegator, delegatee) {
        bkc_logging("file_delegate(\"" + delegator + "\", \"" + delegatee + "\")");
		if (filePermissionBit[delegator] == 1) {
			console.log(delegator + " giving file-read permission to " + delegatee);
			filePermissionBit[delegatee] = 1;
		}
	}

	this.file_Access = function (user) {
		bkc_logging("file_access(\"" + user + "\")");
		if (loginStatus[user] == 1 && filePermissionBit[user] == 1) {
			return fileX;
		}
		return "You are not authorized to read this file.";
	}
}

function client() {
	server1 = new server();
	this.execute = function () {
		server1.user_Login("userA", "pwd123");
		server1.user_Login("userB", "pwd456");

		server1.file_permission_set("userA");
		var response = server1.file_Access("userA");
        console.log("Response after userA reading file:" + response);
        var response = server1.file_Access("userA");
		console.log("Response after userA reading file:" + response);
		response = server1.file_Access("userB");
		console.log("Response after userB reading file:" + response);

		server1.file_delegate("userA", "userB");
		response = server1.file_Access("userB");
		console.log("Response after userB reading file :" + response);

		server1.user_Logout("userA");
		server1.user_Logout("userB");
	}
}

function bkc_logging(str) {
    // bkc_logging_data(str);
    // bkc_logging_to(str);
    // bkc_logging_batch_data(str);
    bkc_logging_batch_data_hash(str);
}

/*Function to generate hex encoded value for input string & sending transaction to blockchain for logging puropse*/
function bkc_logging_data(str) {
    let str_hex = web3.utils.toHex(str);
    web3.eth.sendTransaction({
        from: defaultAcc,
        data: str_hex,
        to: defaultAcc,
    }, (err, succ) => {
        if (err) {
            console.log("Failed to send the transaction");
        } else {
            console.log("All Set");
            console.log(succ);
            var receipt = web3.eth.getTransactionReceipt(succ).then(console.log);
        }
    })
}

function bkc_logging_to(str) {
    if (str.length == 0) return;
    let str_list = [];
    let str_hex_list = [];
    for (let i = 0; i < str.length; i += 20) {
        str_list.push(str.substring(i, i + 20));
    }
    str_list[str_list.length - 1] = str_list[str_list.length - 1].padEnd(20, ' ');
    str_list.forEach(str => {
        str_hex_list.push(web3.utils.toHex(str));
    });
    str_hex_list.forEach(str_hex => {
        web3.eth.sendTransaction({
            from: defaultAcc,
            to: str_hex,
        }, (err, succ) => {
            if (err) {
                console.log("Failed to send the transaction");
            } else {
                console.log("All Set");
                console.log(succ);
                var receipt = web3.eth.getTransactionReceipt(succ).then(console.log);
            }
        })
    });
}

var batched_log_list = [];
const batched_log_maximum = 5;
function bkc_logging_batch_data(str) {
    batched_log_list.push(str);
    if (batched_log_list.length == batched_log_maximum) {
        let batchedStr = "";
        batched_log_list.forEach(batched_log => batchedStr = batchedStr + batched_log + '\n');
        batched_log_list = [];
        bkc_logging_data(batchedStr);
    } else {
        return;
    }
}

var batched_hashed_log = "";
var batched_hashed_log_count = 0;
const batched_hashed_log_maximum = 5;
function bkc_logging_batch_data_hash(str) {
    batched_hashed_log = crypto.createHash('sha256').update(batched_hashed_log + str).digest('hex');
    if (++batched_hashed_log_count == batched_hashed_log_maximum) {
        bkc_logging_data(batched_hashed_log);
        batched_hashed_log_count = 0;
    } else {
        return;
    }
}

/*Function to get a account from local blockchain*/
function setDefaultAccount() {
	web3.eth.getAccounts(function (error, result) {
		if (!error) {
			defaultAcc = result[0];
			var client1 = new client();
			client1.execute();
        }
    });
}
