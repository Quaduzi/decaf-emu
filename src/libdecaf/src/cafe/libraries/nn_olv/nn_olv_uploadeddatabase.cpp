#include "nn_olv.h"
#include "nn_olv_uploadeddatabase.h"

#include "cafe/libraries/cafe_hle_stub.h"
#include "nn/olv/nn_olv_result.h"

using namespace nn::olv;

namespace cafe::nn_olv
{

virt_ptr<hle::VirtualTable>
UploadedDataBase::VirtualTable = nullptr;

virt_ptr<hle::TypeDescriptor>
UploadedDataBase::TypeDescriptor = nullptr;

UploadedDataBase::UploadedDataBase() :
   mFlags(0u),
   mBodyTextLength(0u),
   mBodyMemoLength(0u),
   mAppDataLength(0u),
   mFeeling(int8_t { 0 }),
   mCommonDataUnknown(0u),
   mCommonDataLength(0u)
{
   mPostID[0] = char { 0 };
   mVirtualTable = UploadedDataBase::VirtualTable;
}

UploadedDataBase::~UploadedDataBase()
{
}

uint32_t
UploadedDataBase::GetAppDataSize()
{
   if (!TestFlags(HasAppData)) {
      return 0;
   }

   return mAppDataLength;
}

nn::Result
UploadedDataBase::GetAppData(virt_ptr<uint8_t> buffer,
                             virt_ptr<uint32_t> outDatasize,
                             uint32_t bufferSize)
{
   if (!TestFlags(HasAppData)) {
      return ResultNoData;
   }

   if (!buffer) {
      return ResultInvalidPointer;
   }

   if (!bufferSize) {
      return ResultInvalidSize;
   }

   auto length = std::min<uint32_t>(bufferSize, mAppDataLength);
   std::memcpy(buffer.get(),
               virt_addrof(mAppData).get(),
               length);

   if (outDatasize) {
      *outDatasize = length;
   }

   return ResultSuccess;
}

nn::Result
UploadedDataBase::GetBodyText(virt_ptr<char16_t> buffer,
                              uint32_t bufferSize)
{
   if (!TestFlags(HasBodyText)) {
      return ResultNoData;
   }

   if (!buffer) {
      return ResultInvalidPointer;
   }

   if (!bufferSize) {
      return ResultInvalidSize;
   }

   auto length = std::min<uint32_t>(bufferSize, mBodyTextLength);
   std::memcpy(buffer.get(),
               virt_addrof(mBodyText).get(),
               length * sizeof(char16_t));

   if (length < bufferSize) {
      buffer[length] = char16_t { 0 };
   }

   return ResultSuccess;
}

nn::Result
UploadedDataBase::GetBodyMemo(virt_ptr<uint8_t> buffer,
                              virt_ptr<uint32_t> outMemoSize,
                              uint32_t bufferSize)
{
   if (!TestFlags(HasBodyMemo)) {
      return ResultNoData;
   }

   if (!buffer) {
      return ResultInvalidPointer;
   }

   if (!bufferSize) {
      return ResultInvalidSize;
   }

   auto length = std::min<uint32_t>(bufferSize, mBodyMemoLength);
   std::memcpy(buffer.get(),
               virt_addrof(mBodyMemo).get(),
               length);

   if (outMemoSize) {
      *outMemoSize = length;
   }

   return ResultSuccess;
}

nn::Result
UploadedDataBase::GetCommonData(virt_ptr<uint32_t> unk,
                                virt_ptr<uint8_t> buffer,
                                virt_ptr<uint32_t> outDataSize,
                                uint32_t bufferSize)
{
   if (!mCommonDataLength) {
      return ResultNoData;
   }

   if (!buffer) {
      return ResultInvalidPointer;
   }

   if (!bufferSize) {
      return ResultInvalidSize;
   }

   auto length = std::min<uint32_t>(bufferSize, mCommonDataLength);
   std::memcpy(buffer.get(),
               virt_addrof(mCommonData).get(),
               length);

   if (unk) {
      *unk = mCommonDataUnknown;
   }

   if (outDataSize) {
      *outDataSize = length;
   }

   return ResultSuccess;
}

int32_t
UploadedDataBase::GetFeeling()
{
   return mFeeling;
}

virt_ptr<const char>
UploadedDataBase::GetPostId()
{
   return virt_addrof(mPostID);
}

bool
UploadedDataBase::TestFlags(uint32_t flag)
{
   return !!(mFlags & flag);
}

void
Library::registerUploadedDataBaseSymbols()
{
   RegisterDestructorExport("__dt__Q3_2nn3olv16UploadedDataBaseFv",
                            UploadedDataBase);
   RegisterFunctionExportName("GetCommonData__Q3_2nn3olv16UploadedDataBaseFPUiPUcT1Ui",
                              &UploadedDataBase::GetCommonData);

   registerTypeInfo<UploadedDataBase>(
      "nn::olv::UploadedDataBase",
      {
         "__pure_virtual_called",
      });
}

}  // namespace cafe::nn_olv
