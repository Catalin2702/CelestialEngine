//
// Module: CelestialEngine/Engine/Modules/Utility/FileSystem
// File: File.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#include "Utility/FileSystem/File.hpp"

#include "Tools/Log/Log.hpp"
#include "Utility/FileSystem/FileSystem.hpp"

#include <utility>

namespace CE::Utility {

File::File(fs::path  path, const Types::FileLoadState loadState, const bool autoSave):
_path(std::move(path)),
_loadState(loadState),
_autoSave(autoSave) {
	if (loadState == Types::FileLoadState::NotLoaded)
		Load();
}

File::File(const File& other): _path(other._path), _autoSave(other._autoSave) {}

File::File(File&& other) noexcept {
	_path = std::move(other._path);
	_content = std::move(other._content);
	_loadState = other._loadState;
	_autoSave = other._autoSave;
	_isLoaded = other._isLoaded;
	_isChanged = other._isChanged;
	_isSaved = other._isSaved;

	// Without this, a moved-from File (e.g. the local NRVO/move-elided source when a factory
	// function like FileSystem::StCreate returns by value without the compiler eliding the move -
	// MSVC's /Od reliably skips NRVO where Clang/GCC often still perform it even unoptimized, which
	// is why this only ever surfaced on the Visual Studio generator) keeps _autoSave/_isChanged
	// from the source. Its destructor then still tries to auto-save, but _path was just moved out
	// (now empty), so FileSystem::StSave throws - out of a noexcept destructor, which terminates
	// the process instead of throwing a catchable exception.
	other._autoSave = false;
	other._isChanged = false;
	other._isSaved = false;
}

File::~File() {
	if (_autoSave && _isChanged) {
		FileSystem::StSave(*this);
	}
}

File& File::operator=(const File& other) {
	if (this == &other) [[unlikely]]
		return *this;

	_path = other._path;
	_loadState = other._loadState;
	_autoSave = other._autoSave;
	_isLoaded = other._isLoaded;
	_isChanged = other._isChanged;
	_isSaved = other._isSaved;

	return *this;
}

File& File::operator=(File&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	_path = std::move(other._path);
	_content = std::move(other._content);
	_loadState = other._loadState;
	_autoSave = other._autoSave;
	_isLoaded = other._isLoaded;
	_isChanged = other._isChanged;
	_isSaved = other._isSaved;

	// See the move constructor's comment: a moved-from File must not attempt to auto-save on
	// destruction, since its _path was just moved out.
	other._autoSave = false;
	other._isChanged = false;
	other._isLoaded = false;

	return *this;
}

void File::Load() {
	FileSystem::StLoad(*this);
	_isLoaded = true;
	_isChanged = false;
	_isSaved = true;
}

void File::UnLoad() {
	FileSystem::StUnload(*this);
	_isLoaded = false;
	_isChanged = false;
	_isSaved = false;
}

void File::Reload() {
	FileSystem::StReload(*this);
}

void File::Save() const {
	FileSystem::StSave(*this);
	_isSaved = true;
	_isChanged = false;
}

void File::SetContent(const std::string& content) {
	_content = std::vector<uint8_t>(content.begin(), content.end());
	_isLoaded = true;
	_isChanged = true;
	_isSaved = false;
}

void File::SetContent(const std::vector<uint8_t>& content) {
	_content = content;
	_isLoaded = true;
	_isChanged = true;
	_isSaved = false;
}

void File::SetContent(std::vector<uint8_t>&& content) {
	_content = std::move(content);
	_isLoaded = true;
	_isChanged = true;
	_isSaved = false;
}

std::string File::GetContentStringLazy() {
	if (not _isLoaded)
		FileSystem::StLoad(*this);
	return {_content.begin(), _content.end()};
}

const std::vector<uint8_t>& File::GetContentBytesLazy() {
	if (not _isLoaded)
		FileSystem::StLoad(*this);
	return _content;
}

std::string File::GetContentString() const {
	if (not _isLoaded) {
		CE_CORE_WARN("File::GetContentString: Attempted to get content string from file at path: {0} but the content is not loaded. Returning empty string.", _path.string());
		return {};
	}
	return {_content.begin(), _content.end()};
}

const std::vector<uint8_t>& File::GetContentBytes() const {
	return _content;
}

bool File::Exists() const {
	return FileSystem::StExists(_path);
}

}
