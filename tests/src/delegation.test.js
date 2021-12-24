import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] delegation', zemu("nanos", async (sim, lat) => {
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f87e168502540be4008303345094100000000000000000000000000000000000000280b858f856838203ec8180b842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c820554990508b8a0273c09d4e3d882503ae648080',
  );

  await waitForAppScreen(sim);
  
  try{
    await sim.navigateAndCompareSnapshots('.', 'nanos_delegation', [17, 0]);
  } catch (e) {
    console.error(e);
  }
  

  await expect(tx).resolves.toEqual({
    "r": "3ee3fc6789b9edf0f72986676dc290ef14ba6d3f0b81d37164f0d4e7e1c10694",
    "s": "66472c3f34a725c8862a4c44b5c942497ebbd27e026d080199d89e500ba7618c",
    "v": "ec",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f87e168502540be4008303345094100000000000000000000000000000000000000280b858f856838203ec8180b842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c820554990508b8a0273c09d4e3d882503ae648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "3ee3fc6789b9edf0f72986676dc290ef14ba6d3f0b81d37164f0d4e7e1c10694",
//     "s": "66472c3f34a725c8862a4c44b5c942497ebbd27e026d080199d89e500ba7618c",
//     "v": "ec",
//   });
// }));
