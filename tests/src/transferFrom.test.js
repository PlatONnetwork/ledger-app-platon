import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] TransferFrom prc721 tokens', zemu("nanos", async (sim, lat) => {
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f88a2a8502540be400830334509424bc06168d9f62eaee5fed494c142064af23f70e80b86423b872dd000000000000000000000000037440bf82c666042b6611acef0de5c35177ed43000000000000000000000000c115ceadf9e5923330e5f42903fe7f926dda65d200000000000000000000000000000000000000000000000000000000000f4243648080',
  );

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_transferFrom', [13, 0]);

  await expect(tx).resolves.toEqual({
    "r": "c1338c3620b5bdab4c04de052e245685145152e60bc770e1891eda583605f384",
    "s": "6fa340e867acbfe8445b3e7d31361ccc0c35a10817632e7016aa80febd7b3d70",
    "v": "ec",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f88a2a8502540be400830334509424bc06168d9f62eaee5fed494c142064af23f70e80b86423b872dd000000000000000000000000037440bf82c666042b6611acef0de5c35177ed43000000000000000000000000c115ceadf9e5923330e5f42903fe7f926dda65d200000000000000000000000000000000000000000000000000000000000f4243648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "c1338c3620b5bdab4c04de052e245685145152e60bc770e1891eda583605f384",
//     "s": "6fa340e867acbfe8445b3e7d31361ccc0c35a10817632e7016aa80febd7b3d70",
//     "v": "ec",
//   });
// }));
