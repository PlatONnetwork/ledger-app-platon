import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu } from './test.fixture';

test('[Nano S] Transfer eip1559', zemu("nanos", async (sim, lat) => {

  const tx = lat.signTransaction(
    "44'/486'/0'/0'/0",
    'e8018504e3b292008252089428ee52a8f3d6e5d15f8b131996950d7f296c7952872bd72a2487400080',
  );

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_eip1559', [7, 0]);

  await expect(tx).resolves.toEqual({
    "r": "5550769486a0def58e1fbc29ad1db2adfb43678a7f9fa72249e1006abfd4d523",
    "s": "6a65ca5213267b5fb532c6c73fef89591909fd62b234001a03a320236b30237d",
    "v": "1b"
  });

}));

// test('[Nano X] Transfer eip1559', zemu("nanox", async (sim, lat) => {

//   const tx = eth.signTransaction(
//     "44'/486'/0'/0'/0",
//     'e8018504e3b292008252089428ee52a8f3d6e5d15f8b131996950d7f296c7952872bd72a2487400080',
//   );

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_transfer_eip1559', [4, 0]);

//   await expect(tx).resolves.toEqual({
//     "r": "5550769486a0def58e1fbc29ad1db2adfb43678a7f9fa72249e1006abfd4d523",
//     "s": "6a65ca5213267b5fb532c6c73fef89591909fd62b234001a03a320236b30237d",
//     "v": "1b"
//   });

// }));