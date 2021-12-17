import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] add staking', zemu("nanos", async (sim, lat) => {
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f8872a8502540be4008303345094100000000000000000000000000000000000000280b861f85f838203eab842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c820554990508180949305313a5dee87d6eb0f658d6c3878d37f040000648080',
  );

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_add_staking', [17, 0]);

  await expect(tx).resolves.toEqual({
    "r": "fb739d98f1f8dd50f7d5204bf3e4f953d79451cb348a6eb85d2a58ce2a875d04",
    "s": "2d7dea18367d0d0fe42081305320810330c21b4a9e0a341dfca720a33149c058",
    "v": "eb",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f8872a8502540be4008303345094100000000000000000000000000000000000000280b861f85f838203eab842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c820554990508180949305313a5dee87d6eb0f658d6c3878d37f040000648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "fb739d98f1f8dd50f7d5204bf3e4f953d79451cb348a6eb85d2a58ce2a875d04",
//     "s": "2d7dea18367d0d0fe42081305320810330c21b4a9e0a341dfca720a33149c058",
//     "v": "eb",
//   });
// }));
