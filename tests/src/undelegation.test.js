import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] UnDelegation', zemu("nanos", async (sim, lat) => {
  
  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'f883808502540be4008303345094100000000000000000000000000000000000000280b85af858838203ed8483047d04b842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c820554990508a89063bf212b431ec0000830335f98080',
  );

  await waitForAppScreen(sim);

  try{
    await sim.navigateAndCompareSnapshots('.', 'nanos_undelegation', [16, 0]);
  }catch (e) {
    console.error(e);
  }

  await expect(tx).resolves.toEqual({
    "r": "fe2b557038ef63e8546c4aaba1a7f04d2fbb518295982dc938def75b59d79bd8",
    "s": "2a0a42ddcead261ad05a6a631093d442c7500c942d6061bee24eaad36cd3dbc9",
    "v": "066c15",
  });
}));

// test('[Nano X] Approve DAI tokens', zemu("nanox", async (sim, lat) => {

//   const tx = lat.signTransaction(
//     "44'/486'/0'/0'/0",
//     'f880168502540be4008303345094100000000000000000000000000000000000000280b85af858838203ed84830462dbb842b84077fffc999d9f9403b65009f1eb27bae65774e2d8ea36f7b20a89f82642a5067557430e6edfe5320bb81c3666a19cf4a5172d6533117d7ebcd0f2c820554990508a89063bf212b431ec0000648080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_approve_dai_tokens', [5, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "d2a93db813a358aca66f43baaf607fb11cd8776e4a63c6e0c06211044c33fd7a",
//     "s": "54c8978c353e670ac60bb521043bf7e77082984c48eb51b56dc50b655195fcb5",
//     "v": "eb",
//   });
// }));
