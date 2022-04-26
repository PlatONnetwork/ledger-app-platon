import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] Unstaking', zemu("nanos", async (sim, lat) => {
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f873808502540be4008303345094100000000000000000000000000000000000000280b84af848838203ebb842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c82055499050830335f98080',
  );

  await waitForAppScreen(sim);

  try{
    await sim.navigateAndCompareSnapshots('.', 'nanos_unstaking', [15, 0]);
  }catch (e) {
    console.error(e);
  }

  await expect(tx).resolves.toEqual({
    "r": "0a6e5c0ec5cdab6cd730caf4f7d83e882907652d7eae056508cb3efe1b73847b",
    "s": "33440441756ee69aeb3379ae1c1e75e4582f4785d8a9ac27d05d756611f20578",
    "v": "066c16",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f8702a8502540be4008303345094100000000000000000000000000000000000000280b84af848838203ebb842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c82055499050648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "653e452e08f4d073d854daf321c60b3a7205e226896523b03a1813e03ed0e86a",
//     "s": "1206431c4c93da3b98d23ba0c5bbac4bb61c919f79572297e00683331b29f428",
//     "v": "eb",
//   });
// }));
