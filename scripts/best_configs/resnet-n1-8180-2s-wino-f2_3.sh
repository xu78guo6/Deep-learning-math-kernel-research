#/bin/bash

# Resnet50
# batch-size: 1
# SKX 8180 2S

source ./scripts/best_configs/common.sh $@

# resnet50_res2a_branch2b, 8.4T tflops: 8.27005
NSOCKETS=2 ./scripts/run.sh -c -i64 -h56 -o64 -H56 -n1 --tile-size=4 --execution-mode=0xa040 --blk-i=4 --blk-o=4 --blk-t=14 --pat-o=1 --output-as-blocked=true $COMMON
sleep 1
# resnet50_res3a_branch2b, 6.3Tv tflops:7.44817
NSOCKETS=2 ./scripts/run.sh -c -i128 -h28 -o128 -H28 -n1 --tile-size=4 --execution-mode=0xa061 --blk-i=8 --blk-o=2 --blk-t=7 --pat-o=2 --output-as-blocked=true $COMMON
sleep 1
# resnet50_res4a_branch2b, 5.3T tflops:5.73528
NSOCKETS=2 ./scripts/run.sh -c -i256 -h14 -o256 -H14 -n1 --tile-size=4 --execution-mode=0xa000 --blk-i=2 --blk-o=8 --blk-t=7 --output-as-blocked=true $COMMON
sleep 1
# resnet50_res5a_branch2b, 3.6T tflops:4.3533
NSOCKETS=2 ./scripts/run.sh -c -i512 -h7 -o512 -H7 -n1 --tile-size=4 --execution-mode=0xa000 --blk-i=16 --blk-o=4 --blk-t=8 $COMMON
