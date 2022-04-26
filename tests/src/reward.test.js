import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] "Claim Rewards', zemu("nanos", async (sim, lat) => {
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'ed808502540be400830334509410000000000000000000000000000000000000068085c483821388830335f98080',
  );

  await waitForAppScreen(sim);
  
  try{
    await sim.navigateAndCompareSnapshots('.', 'nanos_reward', [7, 0]);
  }catch (e) {
    console.error(e);
  }

  await expect(tx).resolves.toEqual({
    "r": "8fb1960f87ce016d10a88145d5f7b0a8fa1737d897d58b231de991bc66b86297",
    "s": "47121ab457d53e04355e07954c9be2abf3c0f036c187e8a74032484272d82536",
    "v": "066c15",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'ea168502540be400830334509410000000000000000000000000000000000000068085c483821388648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "d61027e3811ddf55dd90b9910ea7a828b39f3c2cf872c50a7979c66002ec37bf",
//     "s": "1570ded6abe4ef158c309af8e9cd35ffce1dcef749ddaeb122b369cf4336e04a",
//     "v": "eb",
//   });
// }));
