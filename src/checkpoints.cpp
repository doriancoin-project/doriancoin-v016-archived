// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, uint256("0xd21da25e277bd20b7456087d69c5fee2ebc6091b410271b5cb0623c7d1e7d1b9"))
        (  1500, uint256("0xc2418141e7db51d12de03e9e161d3de41f67ea5a9409aa9c9ae41f1c31f661d4"))
        (  4032, uint256("0x81ecfe560526ee5004d4e46d02b4cb32adc7bd9de75a62abe22c58257f7fd5bf"))
        (  8064, uint256("0x1107b7955794a39a54c62207052d5677b6d70f4d5bca0638d5b49aaeecb356c9"))
        ( 16128, uint256("0x3e48a2197c1465aad2c08e8dd5d9f16ffe4e861514452cbd3f74358988c6b487"))
        ( 23420, uint256("0xea49f4f4335fc1c21781fe6be43dc135de4e7321bbe9f5daad2999e909cc07c4"))
        ( 50000, uint256("0xcf26ff7369cee9d04ae5ea9f364a06c9acb2f7739229ea58eed46162ef69397f"))
        ( 80000, uint256("0x5af01844c1f929ea0ea80d1edd9cb8167f743c3b36068e7c3d487e274b84b98b"))
        (120000, uint256("0x3ca98428e1ca069b09ec0cc32b0d24e2b72ac0c02db1250b351272c31a072156"))
        (161500, uint256("0xbff9f62380d99ad42381dd0a004d5ada447e8077b30d25b00f2a8e6a33a9b856"))
        (179620, uint256("0x0120cff29f85e51828a034efe26fa1f48702de7a52b1cc25dd9d6d158f0a3a54"))
        (240000, uint256("0x021f02e45c710343617353ee887592db9a1c966bea66fbe587c25c63f2987e92"))
       // (383640, uint256("0x2b6809f094a9215bafc65eb3f110a35127a34be94b7d0590a096c3f126c6f364"))
       // (409004, uint256("0x487518d663d9f1fa08611d9395ad74d982b667fbdc0e77e9cf39b4f1355908a3"))
       // (456000, uint256("0xbf34f71cc6366cd487930d06be22f897e34ca6a40501ac7d401be32456372004"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1383887110, // * UNIX timestamp of last checkpoint block
        2179203,    // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        8000.0     // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet = 
        boost::assign::map_list_of
        (   546, uint256("000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70"))
        ( 35000, uint256("2af959ab4f12111ce947479bfcef16702485f04afd95210aa90fde7d1e4a64ad"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1369685559,
        37581,
        300
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
