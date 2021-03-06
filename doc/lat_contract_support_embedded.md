# Platon application : Embedded Contract Support

## About

This document described how a specific device UI for a smart contract can be added in the current version of the Platon application, before plugins are added

## Standard support

The applications already includes dedicated UI support for those specific contract calls :

* PRC 20 approve(address, uint256) - implementation in *src_features/prc20_approval*
* PRC 20 transfer(address, uint256) - implementation in *src_features/signTx*

## Requirements

The following data is necessary for a specific contract support

* Smart contract ABI (at least for calls that are to be supported by the application)

The following data is optional for a specific contract support

* Contract address (can be optional if supported by multiple instances)

### Computing a function selector

A function selector is computed as in https://solidity.readthedocs.io/en/v0.5.3/abi-spec.html#function-selector

```
The first four bytes of the call data for a function call specifies the function to be called. It is the first (left, high-order in big-endian) four bytes of the Keccak-256 (SHA-3) hash of the signature of the function. The signature is defined as the canonical expression of the basic prototype without data location specifier, i.e. the function name with the parenthesised list of parameter types. Parameter types are split by a single comma - no spaces are used.
```

The following online tool can be used to compute selectors https://emn178.github.io/online-tools/keccak_256.html

## Limitations

* The total number of parameters of a contract call is 4 (as defined in *src/shared_context.h* for tokenContext_t.data, each parameter being encoded as an uint256). Additional parameters can be supported if not required to handle the displaying logic (see "Also handle exception that only need to process the beginning of data" in *src_features/signTx/logic_signTx.c*)

* Non fixed size types (dynamic sized byte sequences, dynamic sized strings, variable length arrays) are not directly supported

## Sample implementation of a function call

This example describes how to implement a specific UI for a contract call. This exemple is using Starkex token deposit - *deposit(uint256 tokenId, uint256 vaultId, uint256 quantizedAmount)*

* Implement the specific UI logic in a new *src_features* subdirectory - note that strings.common.maxFee will be provisioned with the maximum fee to pay for this call

```
See src_features/stark_contract_deposit for this example
```

* Compute the selector

```
keccak-256("deposit(uint256,uint256,uint256)") = 00aeef8a...
```  

* Add an entry to the contract_call_t enum of *src/shared_context.h* for this function call

```
CONTRACT_STARKWARE_DEPOSIT_TOKEN
```

* Check for the selector being called and the arguments size in *src_features/signTx/logic_signTx.c* customProcessor function - additional logic can be applied (quantumSet in this example)

```C
if ((context->currentFieldLength == STARKWARE_DEPOSIT_TOKEN_DATA_SIZE) &&
    (memcmp(context->workBuffer, STARKWARE_DEPOSIT_TOKEN_ID, 4) == 0) &&
    quantumSet) {
   contractProvisioned = CONTRACT_STARKWARE_DEPOSIT_TOKEN;
}
```

* Call the dedicated UI at the end of the transaction parsing in *src_features/signTx/logic_signTx.c* finalizeParsing function

```C++
if (contractProvisioned == CONTRACT_STARKWARE_DEPOSIT_TOKEN) {
   ux_flow_init(0, ux_approval_starkware_deposit_flow, NULL);
   return;
}
```  

## Using PRC 20 token tickers

A UI implementation might want to convert an PRC 20 token contract address to a ticker for easier validation

2 tickers can be temporarily provisioned to the application by using the PROVIDE PRC 20 TOKEN INFORMATION APDU, described in *src_features/provideErc20TokenInformation* - the UI can then iterate on the provisioned tickers to display relevant information to the user

The same mechanism will be extended to support well known contract addresses in the future
