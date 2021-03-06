import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] transfer prc20 tokens', zemu("nanos", async (sim, lat) => {

  const TokenInfo = {
    data: Buffer.from("0850524332302d436456bc6e89ed4a0c2156f22729624319dc2bf255fa00000012000335f930450221009e5187dbc026f9cec6c0a4bd28ffcda5d0e6faa12c0fdd599e9c993caa5e09260220403711bd3fcf307bc1f5cb76da8cad288646c91e0e9d70c7ad21be0affa4d7f8", "hex")
  };

  await lat.providePRC20TokenInformation(TokenInfo);
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f86d808502540be400830334509456bc6e89ed4a0c2156f22729624319dc2bf255fa80b844a9059cbb000000000000000000000000c115ceadf9e5923330e5f42903fe7f926dda65d200000000000000000000000000000154484932d2e725a5bbca17a3aba173d3d5830335f98080',
  );

  await waitForAppScreen(sim);

  try{
    await sim.navigateAndCompareSnapshots('.', 'nanos_transfer', [9, 0]);
  } catch (e) {
    console.error(e);
  }

  await expect(tx).resolves.toEqual({
    "r": "a6a7cba3143fff3e0b5e5f944198292b513c6c5546bedcb037591d748a0305c2",
    "s": "3c78782527a3fd5827284988b93742602c7bca6e007d1a962a51c3ec285908d3",
    "v": "066c15",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   TokenInfo = {
//     data: Buffer.from("0850524332302d436456bc6e89ed4a0c2156f22729624319dc2bf255fa00000012000000643045022100f2daf653c4ec97cd5baca6de7fe19a0043fdbbd349e10d4e94b664042e668836022031f620a339543f24cfd1346219fe078f2a0d21fbf21e4496a4d803d0452b49b8", "hex")
//   };

//   lat.providePRC20TokenInformation(TokenInfo);

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f86a2a8502540be400830334509456bc6e89ed4a0c2156f22729624319dc2bf255fa80b844a9059cbb000000000000000000000000c115ceadf9e5923330e5f42903fe7f926dda65d200000000000000000000000000000154484932d2e725a5bbca17a3aba173d3d5648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "316fc3eb9d6b64ec992762b16892a169390bb728ec89e1f78eb190a868ebb48a",
//     "s": "698cdb8eb364ba2279a8885a623e527bf88c586221b789e4f905f9b1558249be",
//     "v": "eb",
//   });
// }));
