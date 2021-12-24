import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] Approve prc20 token', zemu("nanos", async (sim, lat) => {

  const TokenInfo = {
    data: Buffer.from("0850524332302d436456bc6e89ed4a0c2156f22729624319dc2bf255fa00000012000000643045022100f2daf653c4ec97cd5baca6de7fe19a0043fdbbd349e10d4e94b664042e668836022031f620a339543f24cfd1346219fe078f2a0d21fbf21e4496a4d803d0452b49b8", "hex")
  };

  await lat.providePRC20TokenInformation(TokenInfo);
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f86a2a8502540be400830334509456bc6e89ed4a0c2156f22729624319dc2bf255fa80b844095ea7b3000000000000000000000000c115ceadf9e5923330e5f42903fe7f926dda65d2fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe648080',
  );

  await waitForAppScreen(sim);

  try{
    await sim.navigateAndCompareSnapshots('.', 'nanos_approve', [12, 0]);
  }catch (e) {
    console.error(e);
  }

  await expect(tx).resolves.toEqual({
    "r": "334e8e60fc79ae9fc8edce769985d0da9c6455348f74750c043ba547534f4bc5",
    "s": "6dcabcebbf249a22334c4658d30dbe867241e7416466f2c738260e9cee59a0b2",
    "v": "eb",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   TokenInfo = {
//     data: Buffer.from("0850524332302d436456bc6e89ed4a0c2156f22729624319dc2bf255fa00000012000000643045022100f2daf653c4ec97cd5baca6de7fe19a0043fdbbd349e10d4e94b664042e668836022031f620a339543f24cfd1346219fe078f2a0d21fbf21e4496a4d803d0452b49b8", "hex")
//   };

//   lat.providePRC20TokenInformation(TokenInfo);

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f86a2a8502540be400830334509456bc6e89ed4a0c2156f22729624319dc2bf255fa80b844095ea7b3000000000000000000000000c115ceadf9e5923330e5f42903fe7f926dda65d2fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "334e8e60fc79ae9fc8edce769985d0da9c6455348f74750c043ba547534f4bc5",
//     "s": "6dcabcebbf249a22334c4658d30dbe867241e7416466f2c738260e9cee59a0b2",
//     "v": "eb",
//   });
// }));
