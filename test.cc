#include <iomanip>
#include <random>

#include "knowhere/comp/index_param.h"
#include "knowhere/comp/knowhere_config.h"
#include "knowhere/comp/local_file_manager.h"
#include "knowhere/dataset.h"
#include "knowhere/index/index_factory.h"
#include "knowhere/version.h"
knowhere::DataSetPtr
GenDataSet(int rows, int dim) {
    std::mt19937 rng(42);
    std::uniform_real_distribution<> distrib(-1.0, 1.0);
    float* ts = new float[rows * dim];
    for (int i = 0; i < rows * dim; ++i) {
        ts[i] = (float)distrib(rng);
    }
    auto ds = knowhere::GenDataSet(rows, dim, ts);
    return ds;
}

int
main() {
    auto version = knowhere::Version::GetCurrentVersion().VersionNumber();
    knowhere::Json json;
    auto dim = 512;
    auto topk = 10;
    json[knowhere::meta::DIM] = dim;
    json[knowhere::meta::METRIC_TYPE] = knowhere::metric::L2;
    json[knowhere::meta::TOPK] = topk;
    json[knowhere::indexparam::NLIST] = 1024;
    std::cout << json.dump() << std::endl;

    auto xb_ds = GenDataSet(100000, 512);
    knowhere::Index<knowhere::IndexNode> idx1 =
        knowhere::IndexFactory::Instance()
            .Create<knowhere::fp32>(knowhere::IndexEnum::INDEX_FAISS_IVFFLAT, version)
            .value();

    auto handle = idx1.BuildAsync(xb_ds, json, std::chrono::seconds(50));
    std::this_thread::sleep_for(std::chrono::seconds(5));
    handle->Stop();
    std::cout << (int)handle->Get() << std::endl;

    auto xq_ds = GenDataSet(10, 512);
    auto ret = idx1.Search(xq_ds, json, nullptr);
    auto value = ret.value();
    auto ids = value->GetIds();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            std::cout << (ids[i * 10 + j]) << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
